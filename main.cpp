#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/states/Title.cpp"
#include "src/Constants.h"
#include "src/backend/UIHelpers.cpp"

int main () {
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");
    SetExitKey(KEY_NULL);
    InitAudioDevice();

    // Ładowanie globalnych zasobów audio
    Music globalBGM = LoadMusicStream(UI::AssetPath("music/bgm.mp3"));
    PlayMusicStream(globalBGM);
    UI::LoadUISounds();

    StateManager::Instance().ChangeState(new Title());

    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        StateManager::Instance().ProcessStateChange();

        UpdateMusicStream(globalBGM);

        float dt = GetFrameTime();
        StateManager::Instance().Update(dt);

        BeginDrawing();
            ClearBackground(BLACK);
            StateManager::Instance().Draw();
        EndDrawing();
    }

    UI::UnloadUISounds();
    UnloadMusicStream(globalBGM);
    CloseAudioDevice();

    CloseWindow();

    return 0;
}
