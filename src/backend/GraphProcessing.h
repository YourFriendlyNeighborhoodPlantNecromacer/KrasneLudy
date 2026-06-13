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

inline std::vector<Road> roadConnections;
inline std::vector<RimPoint> rimPoints;
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

    measure("Huffman Compression", [&]() {
        huffman_compression(directory + outputFileForWorkplaceAssignment, directory + "compressed_" + outputFileForWorkplaceAssignment);
        huffman_compression(directory + outputFileForRimPoints, directory + "compressed_" + outputFileForRimPoints);
    });

    InitGraphFilter();
    measure("Loading Assignments From File", [&]() { LoadAssignments(&kingdom, baseFileName); });
    measure("Loading Rim Points From File", [&]() { LoadRimPoints(&kingdom, baseFileName); });

    LogTime("TOTAL TIME TAKEN:", startTotal, "[INFO]");
    std::cout << std::endl;
}

#endif // BACKEND_GRAPH_PROCESSING_H