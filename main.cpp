#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/backend/SettingsManager.h"
#include "src/states/Title.h"
#include "src/backend/GraphProcessing.h"
#include "src/Constants.h"
#include "src/backend/UIHelpers.h"
#include "GLUGLU/functions/country_generator.h"
#include <filesystem>
#include "GLUGLU/functions/huffman_compression.h"

void calculationsAndLogis(country& kingdom, const std::string baseFileName){
    const std::string directory = baseFileName + "/";
    const std::string outputFileForWorkplaceAssignment = baseFileName + "_dwarf_workplace_assignment.txt";
    const std::string outputFileForRimPoints = baseFileName + "_rim_variables.txt";
    kingdom.constructFromFile(baseFileName + ".txt");
    kingdom.assignDwarfsToWorkplaces(directory + outputFileForWorkplaceAssignment);
    kingdom.saveActiveWorkplaces(kingdom.constructRimAroundCountry(), directory  + outputFileForRimPoints);
    std::cout << "AAAA" << std::endl;
    huffman_compression(directory + outputFileForWorkplaceAssignment, directory + "compressed_" + outputFileForWorkplaceAssignment);
    huffman_compression(directory + outputFileForRimPoints, directory + "compressed_" + outputFileForRimPoints);

    InitGraphFilter();
    LoadAssignments(&kingdom, baseFileName);
    LoadRimPoints(&kingdom, baseFileName);
}

void render_and_graphics(country& kingdom, float screenWidth, float screenHeight, const char* windowName){
    SettingsManager::Instance().LoadSettings();

    InitWindow(screenWidth, screenHeight, windowName);
    SetExitKey(KEY_NULL);
    InitAudioDevice();

    // Zastosuj ustawienia do gry
    SettingsManager::Instance().GetSettings().ApplyToGame();

    // Ładowanie globalnych zasobów audio
    Music globalBGM = LoadMusicStream(UI::AssetPath("music/bgm.mp3"));
    PlayMusicStream(globalBGM);
    UI::LoadUISounds();

    StateManager::Instance().ChangeState(new Title(&kingdom));

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        StateManager::Instance().ProcessStateChange();

        UpdateMusicStream(globalBGM);
        // Dynamiczna aktualizacja głośności muzyki z uwzględnieniem ustawień
        SetMusicVolume(globalBGM, SettingsManager::Instance().GetSettings().musicVolume);

        float deltaTime = GetFrameTime();
        StateManager::Instance().Update(deltaTime, &kingdom);

        BeginDrawing();
            ClearBackground(BLACK);
            StateManager::Instance().Draw();

            if (SettingsManager::Instance().GetSettings().showFPS) {
                DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, LIME);
            }
        EndDrawing();
    }

    StateManager::Instance().Shutdown();
    UI::UnloadUISounds();
    UnloadMusicStream(globalBGM);
    CloseAudioDevice();

    CloseWindow();
}

int main (int argc, char** argv) {
    if(argc > 2){
        std::cerr << "WRONG AMOUNT OF ARGUMENTS, PLEASE INSTERT A SINGLE FILE";
        return -1;
    }
    country kingdomCountry;
    std::string baseFileName;
    if(argc == 2){ ///  Został podany plik
        baseFileName = argv[1];
    }
    if(argc == 1){ /// Nie został podany plik
        baseFileName = "randomly_generated";
        int64_t precisionPoint = 3;
        int64_t amountOfWorkplacesPerMaterial = 3;
        int64_t amountOfHousesPerMaterial = 3;
        int64_t capacity = 3;
        bool randomized = true;
        int64_t seed = 3;
        generateCountry(baseFileName + ".txt", precisionPoint, amountOfWorkplacesPerMaterial, amountOfHousesPerMaterial, capacity, randomized, seed);
    }


    if(std::filesystem::create_directories(baseFileName)) std::cout << "WOW";
    calculationsAndLogis(kingdomCountry, baseFileName);

    render_and_graphics(kingdomCountry, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");

    /**/
    return 0;
}
