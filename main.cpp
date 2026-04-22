#include <raylib.h>
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

    Visualization state;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");
    state.Init();

    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        float dt = GetFrameTime();
        state.Update(dt);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            state.Draw();

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
