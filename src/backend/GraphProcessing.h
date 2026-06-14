#ifndef BACKEND_GRAPH_PROCESSING_H
#define BACKEND_GRAPH_PROCESSING_H

#include <raylib.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include "../../DataStructures/country.h"
#include "../Constants.h"
#include "../../GLUGLU/functions/huffman_compression.h"
#include "../../GLUGLU/datastructures/intervalTree.h"
#include <cmath>

struct Road {
    Vector2 start;
    Vector2 end;
    Color color;
    int material;
};

struct RimPoint {
    Vector2 position;
    int material;
};

struct Guard {
    Vector2 position;
    int64_t voicePower;
};

inline std::vector<Road> roadConnections;
inline std::vector<RimPoint> rimPoints;
inline std::vector<Guard> guards;
inline intervalTree* guardsTree = nullptr;
inline std::vector<bool> worldMaterialFilter;

inline void InitGraphFilter() {
    if (worldMaterialFilter.empty()) {
        worldMaterialFilter.assign(namedValues::material::size, true);
    }
}

inline void LoadAssignments(country* mapPointer, const std::string& baseFileName) {
    roadConnections.clear();
    std::string path = baseFileName + "/" + baseFileName + "_dwarf_workplace_assignment.txt";
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    for (int m = 0; m < namedValues::material::size; ++m) {
        while (std::getline(file, line)) {
            if (line == ";;") break;
            if (line.empty()) continue;

            size_t sep = line.find(';');
            if (sep == std::string::npos) continue;

            int hIdx = std::stoi(line.substr(0, sep));
            int wIdx = std::stoi(line.substr(sep + 1));

            const auto& houses = mapPointer->getHouses()[m];
            const auto& workplaces = mapPointer->getWorkplaces()[m];

            if (hIdx <= houses.getLastIndex() && wIdx <= workplaces.getLastIndex()) {
                auto& house = houses[hIdx];
                auto& wp = workplaces[wIdx];
                if (house && wp) {
                    Vector2 start = { (float)house->coordinates[namedValues::axis::X], (float)house->coordinates[namedValues::axis::Y] };
                    Vector2 end = { (float)wp->coordinates[namedValues::axis::X], (float)wp->coordinates[namedValues::axis::Y] };
                    Color color = Config::MATERIAL_COLORS.at(static_cast<namedValues::material>(m));
                    roadConnections.push_back({ start, end, color, m });
                }
            }
        }
    }
}

inline void LoadRimPoints(country* mapPointer, const std::string& baseFileName) {
    rimPoints.clear();
    std::string path = baseFileName + "/" + baseFileName + "_rim_variables.txt";
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t sep = line.find(';');
        if (sep == std::string::npos) continue;

        int matIDX = std::stoi(line.substr(0, sep));
        int workIDX = std::stoi(line.substr(sep + 1));

        const auto& workplace = mapPointer->getWorkplaces()[matIDX][workIDX];
        if (workplace) {
            Vector2 pos = { (float)workplace->coordinates[namedValues::axis::X], (float)workplace->coordinates[namedValues::axis::Y] };
            rimPoints.push_back({ pos, matIDX });
        }
    }
}

inline void LoadGuards(const std::string& baseFileName) {
    guards.clear();
    if (guardsTree) { delete guardsTree; guardsTree = nullptr; }

    std::string path = baseFileName + "/" + baseFileName + "_guards.txt";
    std::ifstream file(path);
    if (!file.is_open()) return;

    dynamicArray<int64_t> powers;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t firstSep = line.find(';');
        size_t secondSep = line.find(';', firstSep + 1);
        if (firstSep == std::string::npos || secondSep == std::string::npos) continue;

        float x = std::stof(line.substr(0, firstSep));
        float y = std::stof(line.substr(firstSep + 1, secondSep - firstSep - 1));
        int64_t power = std::stoll(line.substr(secondSep + 1));

        guards.push_back({ {x, y}, power });
        powers.append(power);
    }

    if (!powers.isEmpty()) {
        guardsTree = new intervalTree(powers);
    }
}

inline float Vector2Dist(Vector2 v1, Vector2 v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return std::sqrt(dx * dx + dy * dy);
}

inline void LogTime(const std::string& label, std::chrono::high_resolution_clock::time_point start, const std::string& prefix = "[LOG]") {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << prefix << " " << label << ": " << duration << " ms" << std::endl;
}

inline void ProcessCountryData(country& kingdom, const std::string& baseFileName) {
    auto startTotal = std::chrono::high_resolution_clock::now();

    const std::string directory = baseFileName + "/";
    const std::string outputFileForWorkplaceAssignment = baseFileName + "_dwarf_workplace_assignment.txt";
    const std::string outputFileForRimPoints = baseFileName + "_rim_variables.txt";
    const std::string outputFileForGuards = baseFileName + "_guards.txt";

    auto measure = [](const std::string& label, auto task) {
        auto start = std::chrono::high_resolution_clock::now();
        task();
        LogTime(label, start);
    };

    std::cout << "[INFO] PROCESSING DATA FOR: " << baseFileName << std::endl;

    measure("Building Country", [&]() { kingdom.constructFromFile(baseFileName + ".txt"); });
    measure("Assigning Dwarfs To Workplaces", [&]() { kingdom.assignDwarfsToWorkplaces(directory + outputFileForWorkplaceAssignment); });

    std::unique_ptr<dynamicArray<couple<namedValues::material, int64_t>>> rim;
    measure("Constructing Rim", [&]() { rim = kingdom.constructRimAroundCountry(); });
    measure("Saving Active Workplaces", [&]() { kingdom.saveActiveWorkplaces(std::move(rim), directory + outputFileForRimPoints); });

    InitGraphFilter();
    measure("Loading Assignments From File", [&]() { LoadAssignments(&kingdom, baseFileName); });
    measure("Loading Rim Points From File", [&]() { LoadRimPoints(&kingdom, baseFileName); });

    measure("Generating Dekametrowcy", [&]() {
        guards.clear();
        if (rimPoints.empty()) return;

        std::ofstream gFile(directory + outputFileForGuards);
        dynamicArray<int64_t> powers;

        for (size_t i = 0; i < rimPoints.size(); ++i) {
            Vector2 p1 = rimPoints[i].position;
            Vector2 p2 = rimPoints[(i + 1) % rimPoints.size()].position;

            float segLen = Vector2Dist(p1, p2) * (Config::MAP_HALF / Config::MAP_DECAMETER_DIVIDE);
            int guardsOnSeg = std::max(1, (int)(segLen / Config::DECAMETER_DISTANCE));

            for (int j = 0; j < guardsOnSeg; ++j) {
                float t = (float)j / (float)guardsOnSeg;
                Vector2 internalPos = { p1.x + (p2.x - p1.x) * t, p1.y + (p2.y - p1.y) * t };

                // Normalizuj do zakresu [0, 1] dla zapisu do pliku
                Vector2 normalizedPos = { (internalPos.x + 1.0f) / 2.0f, (internalPos.y + 1.0f) / 2.0f };

                int64_t power = GetRandomValue(20, 100); // Głośność
                guards.push_back({ normalizedPos, power });
                powers.append(power);
                gFile << normalizedPos.x << ";" << normalizedPos.y << ";" << power << "\n";
            }
        }
        gFile.close();

        if (guardsTree) delete guardsTree;
        guardsTree = new intervalTree(powers);
    });

    measure("Huffman Compression", [&]() {
        huffman_compression(directory + outputFileForWorkplaceAssignment, directory + "compressed_" + outputFileForWorkplaceAssignment);
        huffman_compression(directory + outputFileForRimPoints, directory + "compressed_" + outputFileForRimPoints);
        huffman_compression(directory + outputFileForGuards, directory + "compressed_" + outputFileForGuards);
    });

    measure("Loading Guards", [&]() { LoadGuards(baseFileName); });

    LogTime("TOTAL TIME TAKEN:", startTotal, "[INFO]");
    std::cout << std::endl;
}

#endif // BACKEND_GRAPH_PROCESSING_H