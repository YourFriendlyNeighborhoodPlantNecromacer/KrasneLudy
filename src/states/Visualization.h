#ifndef STATE_VISUALIZATION_H
#define STATE_VISUALIZATION_H

#include "../backend/GameState.h"
#include "../backend/StateManager.h"
#include "../backend/UIHelpers.h"
#include <fstream>
#include <algorithm>

#include "../../DataStructures/country.h"

inline void GoToTitle(country* country_ptr);

static inline void DrawWorkplaceMarker(float x, float y, Color fill) {
    const float radius = 32.0f;
    Vector2 top = { x, y - radius };
    Vector2 right = { x + radius, y };
    Vector2 bottom = { x, y + radius };
    Vector2 left = { x - radius, y };

    DrawTriangle(top, bottom, right, fill);
    DrawTriangle(top, left, bottom, fill);

    DrawLineEx(top, right, 3.0f, BLACK);
    DrawLineEx(right, bottom, 3.0f, BLACK);
    DrawLineEx(bottom, left, 3.0f, BLACK);
    DrawLineEx(left, top, 3.0f, BLACK);
}

static inline void DrawHouseMarker(float x, float y, Color fill) {
    const float halfWidth = 32.0f;
    const float bodyHeight = 28.0f;
    const float roofHeight = 28.0f;

    Vector2 left = { x - halfWidth, y };
    Vector2 right = { x + halfWidth, y };
    Vector2 roof = { x, y - roofHeight };
    Vector2 bottomLeft = { x - halfWidth, y + bodyHeight };
    Vector2 bottomRight = { x + halfWidth, y + bodyHeight };

    DrawRectangleRec({ left.x, left.y, halfWidth * 2.0f, bodyHeight }, fill);
    DrawTriangle(roof, left, right, fill);

    DrawLineEx(roof, left, 3.0f, WHITE);
    DrawLineEx(left, bottomLeft, 3.0f, WHITE);
    DrawLineEx(bottomLeft, bottomRight, 3.0f, WHITE);
    DrawLineEx(bottomRight, right, 3.0f, WHITE);
    DrawLineEx(right, roof, 3.0f, WHITE);
}

static inline void DrawCountryEntities(const country* c) {
    const auto& workplaces = c->get_workplaces();
    const auto& houses = c->get_houses();

    for(int material_type = 0; material_type < NamedValues::material::size; material_type++) {
        Color fill = Config::MATERIAL_COLORS.at(static_cast<NamedValues::material>(material_type));

        for(int64_t i = 0; i <= workplaces[material_type].get_last_index(); i++) {
            const auto& wp = workplaces[material_type][i];
            if(!wp) continue;
            DrawWorkplaceMarker((float)wp->coordinates[NamedValues::axis::X] * Config::MAP_HALF, (float)wp->coordinates[NamedValues::axis::Y] * Config::MAP_HALF, fill);
        }

        for(int64_t i = 0; i <= houses[material_type].get_last_index(); i++) {
            const auto& housePtr = houses[material_type][i];
            if(!housePtr) continue;
            DrawHouseMarker((float)housePtr->coordinates[NamedValues::axis::X] * Config::MAP_HALF, (float)housePtr->coordinates[NamedValues::axis::Y] * Config::MAP_HALF, fill);
        }
    }
}

class Visualization : public GameState {
    public:

    Texture2D marble_bg;
    Rectangle viewportArea;
    Camera2D camera;
    Font customFont;

    country* map_pointer;

    Rectangle btnBack = { 0 };
    Rectangle btnLoad = { 0 };
    Rectangle sliderMapSize = { 0 };

    int selectedItem = -1;
    int lastSelectedItem = 0;
    bool isDraggingSlider = false;

    Visualization(country* map_pointer) : map_pointer(map_pointer) {}

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
        sliderMapSize = { btnBack.x + 240.0f, btnBack.y + 15.0f, 300, 10 };

        // Create a small test data file for the country and load it

        customFont = UI::LoadStandardFont(40);
    }

    void Update(float dt, country* country_ptr) override {
        if (IsKeyReleased(KEY_ESCAPE)) {
            UI::PlaySelectSound();
            GoToTitle(map_pointer);
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

        // Clamp camera target to map dimensions
        if (camera.target.x < -Config::MAP_HALF) camera.target.x = -Config::MAP_HALF;
        if (camera.target.x > Config::MAP_HALF) camera.target.x = Config::MAP_HALF;
        if (camera.target.y < -Config::MAP_HALF) camera.target.y = -Config::MAP_HALF;
        if (camera.target.y > Config::MAP_HALF) camera.target.y = Config::MAP_HALF;

        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, btnBack)) selectedItem = 0;
        else if (CheckCollisionPointRec(mouse, btnLoad)) selectedItem = 1;
        else if (CheckCollisionPointRec(mouse, sliderMapSize)) selectedItem = 2;
        else selectedItem = -1;

        if (selectedItem != -1 || isDraggingSlider) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && selectedItem == 2) {
            isDraggingSlider = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isDraggingSlider = false;
        }

        if (isDraggingSlider) {
            float mouseX = std::max(sliderMapSize.x, std::min(GetMousePosition().x, sliderMapSize.x + sliderMapSize.width));
            float progress = (mouseX - sliderMapSize.x) / sliderMapSize.width;
            Config::MAP_SIZE = 500.0f + progress * 9500.0f; // Zakres od 500 do 10000
            Config::MAP_HALF = Config::MAP_SIZE / 2.0f;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, btnBack)) {
                UI::PlaySelectSound();
                GoToTitle(country_ptr);
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
                    for (int i = -Config::MAP_HALF; i <= Config::MAP_HALF; i += 100) {
                        DrawLine(i, -Config::MAP_HALF, i, Config::MAP_HALF, Fade(RAYWHITE, 0.2f));
                        DrawLine(-Config::MAP_HALF, i, Config::MAP_HALF, i, Fade(RAYWHITE, 0.2f));
                    }

                    // Draw country entity map
                    DrawCountryEntities(map_pointer);
            EndMode2D();
        EndScissorMode();

        UI::DrawMenuButton(btnBack, "WYJDŹ", customFont, Color{(148),(0),(0),(255)}, Color{(196),(0),(0),(255)});
        UI::DrawMenuButton(btnLoad, "ŁADUJ GRAF", customFont);

        // Rysowanie Slidera
        DrawRectangleRec(sliderMapSize, DARKGRAY);
        float progress = (Config::MAP_SIZE - 500.0f) / 9500.0f;
        DrawRectangleV({ sliderMapSize.x + progress * sliderMapSize.width - 10, sliderMapSize.y - 10 }, { 20, 30 }, RAYWHITE);
        DrawTextEx(customFont, TextFormat("ROZMIAR MAPY: %.0f", Config::MAP_SIZE), { sliderMapSize.x, sliderMapSize.y + 25 }, 20, 0, RAYWHITE);

        if (selectedItem != -1) {
            Rectangle r = (selectedItem == 0) ? btnBack : (selectedItem == 1 ? btnLoad : sliderMapSize);
            DrawRectangleLinesEx({ r.x - 5, r.y - 5, r.width + 10, r.height + 10 }, 2, Color{96, 96, 0, 255});
        }

        DrawRectangleLinesEx(viewportArea, 3, RAYWHITE);
    }

    ~Visualization() {
        UnloadTexture(marble_bg);
    }
};

#endif // STATE_VISUALIZATION_H
