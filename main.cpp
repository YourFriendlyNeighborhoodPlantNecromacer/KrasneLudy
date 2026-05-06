#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/states/Visualization.cpp"
//#include "src/DataStructures/DataStructures.h"

/*
AXIS REFERENCE:
// positive x moves to right
// positive y moves up
// positive z moves closer
*/

int main () {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");

    // Set the initial state
    StateManager::Instance().ChangeState(new Visualization());

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
