#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/backend/SettingsManager.h"
#include "src/states/Title.h"
#include "src/Constants.h"
#include "src/backend/UIHelpers.h"
#include "GLUGLU/functions/country_generator.h"

void calculationsAndLogis(country& kingdom, const std::string sourceFile, const std::string outputFileForWorkplaceAssignment, const std::string outputFileForRimPoints){
    kingdom.constructFromFile(sourceFile);
    kingdom.assignDwarfsToWorkplaces(outputFileForWorkplaceAssignment);
    kingdom.saveActiveWorkplaces(kingdom.constructRimAroundCountry(), outputFileForRimPoints);
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

int main () {
    country kingdom_country;
    generateCountry("save_file_1.txt", 1, 1, 10, 20, false, 3);
    calculationsAndLogis(kingdom_country, "save_file_1.txt", "dwarf_workplace_assignment_file_1.txt", "rim_constructing_workplaces_file_1.txt");
    render_and_graphics(kingdom_country, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");

    /**/
    return 0;
}
