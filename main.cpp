#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/states/Title.h"
#include "src/Constants.h"
#include "src/backend/UIHelpers.h"
#include "GLUGLU/functions/country_generator.h"
int main () {
    country kingdom_country;
    generate_country("save_file_2.txt", 6, 500, 5000, 20);
    kingdom_country.construct_from_file("save_file_2.txt");
    std::unique_ptr<dynamic_array<couple<NamedValues::material, int64_t>>> rurro = kingdom_country.construct_rim_around_country();


    ///OKNO
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");
    SetExitKey(KEY_NULL);
    InitAudioDevice();

    // Ładowanie globalnych zasobów audio
    Music globalBGM = LoadMusicStream(UI::AssetPath("music/bgm.mp3"));
    PlayMusicStream(globalBGM);
    UI::LoadUISounds();

    StateManager::Instance().ChangeState(new Title(&kingdom_country));

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        StateManager::Instance().ProcessStateChange();

        UpdateMusicStream(globalBGM);

        float dt = GetFrameTime();
        StateManager::Instance().Update(dt, &kingdom_country);

        BeginDrawing();
            ClearBackground(BLACK);
            StateManager::Instance().Draw();
        EndDrawing();
    }

    UI::UnloadUISounds();
    UnloadMusicStream(globalBGM);
    CloseAudioDevice();

    CloseWindow();

    /**/
    return 0;
}
