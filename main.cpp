#include <iostream>
#include <raylib.h>

using namespace std;

int main () {

    const int SCREEN_WIDTH = 600;
    const int SCREEN_HEIGHT = 400;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");
    SetTargetFPS(60);

    while (WindowShouldClose() == false){

        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
