#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/states/Title.cpp"
#include "src/Constants.h"
//#include "src/DataStructures/DataStructures.h"

/*
AXIS REFERENCE:
// positive x moves to right
// positive y moves up
// positive z moves closer
*/

int main () {
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");

    StateManager::Instance().ChangeState(new Title());

    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        StateManager::Instance().ProcessStateChange();

        float dt = GetFrameTime();
        StateManager::Instance().Update(dt);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            StateManager::Instance().Draw();

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
