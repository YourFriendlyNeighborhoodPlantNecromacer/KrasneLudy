#include <raylib.h>
#include "src/backend/GameState.h"
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
    SetTargetFPS(60);

    while (WindowShouldClose() == false){
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
