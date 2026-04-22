#include <iostream>
#include <raylib.h>

#define FOV_ORTHOGRAPHIC 10.0f

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

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, FOV_ORTHOGRAPHIC, CAMERA_ORTHOGRAPHIC };

    while (WindowShouldClose() == false){

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube((Vector3){-4.0f, 0.0f, 2.0f}, 2.0f, 5.0f, 2.0f, RED);

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
