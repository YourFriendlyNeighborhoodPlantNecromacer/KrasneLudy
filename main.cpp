#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/states/Title.cpp"
#include "src/Constants.h"

int main () {
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");
    SetExitKey(KEY_NULL); // Wyłączamy domyślne zamykanie na ESC, aby móc go używać do nawigacji

    StateManager::Instance().ChangeState(new Title());

    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        StateManager::Instance().ProcessStateChange();

        float dt = GetFrameTime();
        StateManager::Instance().Update(dt);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            StateManager::Instance().Draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
