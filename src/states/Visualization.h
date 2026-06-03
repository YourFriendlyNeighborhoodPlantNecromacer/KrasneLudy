#ifndef STATE_VISUALIZATION_H
#define STATE_VISUALIZATION_H

#include <src/backend/GameState.h>
#include <src/backend/StateManager.h>
#include <src/backend/UIHelpers.h>

inline void GoToTitle();

class Visualization : public GameState {
    public:

    Texture2D marble_bg;
    Rectangle viewportArea;
    Camera2D camera;
    Font customFont;

    Rectangle btnBack = { 0 };
    Rectangle btnLoad = { 0 };

    int selectedItem = 0;
    int lastSelectedItem = 0;

    void Init() override {
        marble_bg = LoadTexture(UI::AssetPath("images/ui/marble.png"));

        float padding = 40.0f;
        float bottomSpace = 140.0f;

        viewportArea = {
            padding,
            padding,
            (float)Config::SCREEN_WIDTH - (padding * 2),
            (float)Config::SCREEN_HEIGHT - padding - bottomSpace
        };

        camera.target = { 0, 0 };
        camera.offset = { viewportArea.x + viewportArea.width / 2.0f, viewportArea.y + viewportArea.height / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        btnBack = { viewportArea.x, viewportArea.y + viewportArea.height + 20.0f, 200, 40 };
        btnLoad = { viewportArea.x, btnBack.y + btnBack.height + 10.0f, 200, 40 };

        customFont = UI::LoadStandardFont(40);
    }

    void Update(float dt) override {
        if (IsKeyReleased(KEY_ESCAPE)) {
            UI::PlaySelectSound();
            GoToTitle();
        }

        float panSpeed = 600.0f * dt / camera.zoom;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    camera.target.y -= panSpeed;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  camera.target.y += panSpeed;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  camera.target.x -= panSpeed;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) camera.target.x += panSpeed;

        float keyboardZoomSpeed = 2.0f * dt;
        if (IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_EQUAL)) camera.zoom += keyboardZoomSpeed;
        if (IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_MINUS)) camera.zoom -= keyboardZoomSpeed;

        if (camera.zoom < 0.1f) camera.zoom = 0.1f;
        if (camera.zoom > 10.0f) camera.zoom = 10.0f;

        if (CheckCollisionPointRec(GetMousePosition(), viewportArea)) {
            float wheel = GetMouseWheelMove();
            if (wheel != 0) {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.offset = GetMousePosition();
                camera.target = mouseWorldPos;

                const float zoomSpeed = 0.125f;
                camera.zoom += wheel * zoomSpeed;
                if (camera.zoom < 0.1f) camera.zoom = 0.1f;
                if (camera.zoom > 10.0f) camera.zoom = 10.0f;
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                Vector2 delta = GetMouseDelta();
                camera.target.x -= delta.x / camera.zoom;
                camera.target.y -= delta.y / camera.zoom;
            }
        }

        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, btnBack)) selectedItem = 0;
        else if (CheckCollisionPointRec(mouse, btnLoad)) selectedItem = 1;

        if (CheckCollisionPointRec(mouse, btnBack) || CheckCollisionPointRec(mouse, btnLoad)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, btnBack)) {
                UI::PlaySelectSound();
                GoToTitle();
            } else if (CheckCollisionPointRec(mouse, btnLoad)) {
                UI::PlaySelectSound();
            }
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(marble_bg);

        DrawRectangleRec(viewportArea, Fade(BLACK, 0.15f));

        BeginScissorMode((int)viewportArea.x, (int)viewportArea.y, (int)viewportArea.width, (int)viewportArea.height);
            BeginMode2D(camera);
                for (int i = -2000; i <= 2000; i += 100) {
                    DrawLine(i, -2000, i, 2000, Fade(RAYWHITE, 0.2f));
                    DrawLine(-2000, i, 2000, i, Fade(RAYWHITE, 0.2f));
                }

                DrawCircle(0, 0, 10, RED);
            EndMode2D();
        EndScissorMode();

        UI::DrawMenuButton(btnBack, "WYJDŹ", customFont, Color{(148),(0),(0),(255)}, Color{(196),(0),(0),(255)});
        UI::DrawMenuButton(btnLoad, "ŁADUJ GRAF", customFont);

        Rectangle r = (selectedItem == 0) ? btnBack : btnLoad;
        DrawRectangleLinesEx({ r.x - 5, r.y - 5, r.width + 10, r.height + 10 }, 2, Color{(96),(96),(0),(255)});

        DrawRectangleLinesEx(viewportArea, 3, RAYWHITE);
    }

    ~Visualization() {
        UnloadTexture(marble_bg);
    }
};

#endif // STATE_VISUALIZATION_H
