#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/backend/SettingsManager.h"
#include "src/states/Title.h"
#include "src/Constants.h"
#include "src/backend/UIHelpers.h"
#include "GLUGLU/functions/country_generator.h"
int main () {
    country kingdom_country;
    //generate_country("save_file_2.txt", 5, 5, 10, 4, true, 2);
    kingdom_country.construct_from_file("save_file_2.txt");

    auto active = kingdom_country.get_active_workplaces();
    std::cout << "last_index = "
        << active.get_last_index()
        << std::endl;

    active.dp();

    kingdom_country.assign_dwarfs_to_workplaces("resounts2.txt");

    active = kingdom_country.get_active_workplaces();
    std::cout << "last_index = "
        << active.get_last_index()
        << std::endl;

    active.dp();

    std::unique_ptr<dynamic_array<couple<NamedValues::material, int64_t>>> rim_points;
    rim_points = kingdom_country.construct_rim_around_country();
    if(!rim_points)
    {
        std::cout << "NULLPTR" << std::endl;
        return 0;
    }
    rim_points->dp();

    ///OKNO
    SettingsManager::Instance().LoadSettings();

    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");
    SetExitKey(KEY_NULL);
    InitAudioDevice();

    // Apply settings to game
    SettingsManager::Instance().GetSettings().ApplyToGame();

    // Ładowanie globalnych zasobów audio
    Music globalBGM = LoadMusicStream(UI::AssetPath("music/bgm.mp3"));
    PlayMusicStream(globalBGM);
    UI::LoadUISounds();

    StateManager::Instance().ChangeState(new Title(&kingdom_country));

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        StateManager::Instance().ProcessStateChange();

        UpdateMusicStream(globalBGM);
        // Dynamiczna aktualizacja głośności muzyki z uwzględnieniem ustawień
        SetMusicVolume(globalBGM, SettingsManager::Instance().GetSettings().musicVolume);

        float deltatime = GetFrameTime();
        StateManager::Instance().Update(deltatime, &kingdom_country);

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

    return 0;
}
