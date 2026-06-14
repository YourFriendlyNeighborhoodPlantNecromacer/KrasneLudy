#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/backend/SettingsManager.h"
#include "src/states/Title.h"
#include "src/states/Generation.h"
#include "src/backend/GraphProcessing.h"
#include "src/Constants.h"
#include "src/backend/UIHelpers.h"
#include "GLUGLU/functions/country_generator.h"
#include <filesystem>
#include "GLUGLU/functions/huffman_compression.h"

void render_and_graphics(country& kingdom, float screenWidth, float screenHeight, const char* windowName, bool startWithGeneration){
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

    if (startWithGeneration) {
        StateManager::Instance().ChangeState(new Generation(&kingdom));
    } else {
        StateManager::Instance().ChangeState(new Title(&kingdom));
    }

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
    if(argc == 2){
        baseFileName = std::filesystem::path(argv[1]).stem().string();
        if(std::filesystem::create_directories(baseFileName)) std::cout << "WOW";
        ProcessCountryData(kingdomCountry, baseFileName);
    }

    render_and_graphics(kingdomCountry, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki", (argc == 1));

    return 0;
}
