#ifndef STATE_VISUALIZATION_H
#define STATE_VISUALIZATION_H

#include "../backend/GameState.h"
#include "../backend/StateManager.h"
#include "../backend/UIHelpers.h"
#include "../backend/classes/Button.h"
#include "../backend/classes/Slider.h"
#include <fstream>
#include <algorithm>
#include <cmath>

#include "../../DataStructures/country.h"

inline void GoToTitle(country* country_ptr);

static inline void DrawWorkplaceMarker(float x, float y, Color fill) {
    static constexpr float radius = 32.0f;
    static constexpr float thickness = 3.0f;

    Vector2 top = { x, y - radius };
    Vector2 right = { x + radius, y };
    Vector2 bottom = { x, y + radius };
    Vector2 left = { x - radius, y };

    DrawTriangle(top, bottom, right, fill);
    DrawTriangle(top, left, bottom, fill);

    DrawLineEx(top, right, thickness, BLACK);
    DrawLineEx(right, bottom, thickness, BLACK);
    DrawLineEx(bottom, left, thickness, BLACK);
    DrawLineEx(left, top, thickness, BLACK);
}

static inline void DrawHouseMarker(float x, float y, Color fill) {
    static constexpr float halfWidth = 32.0f;
    static constexpr float bodyHeight = 28.0f;
    static constexpr float roofHeight = 28.0f;
    static constexpr float thickness = 3.0f;

    Vector2 left = { x - halfWidth, y };
    Vector2 right = { x + halfWidth, y };
    Vector2 roof = { x, y - roofHeight };
    Vector2 bottomLeft = { x - halfWidth, y + bodyHeight };
    Vector2 bottomRight = { x + halfWidth, y + bodyHeight };

    DrawRectangleRec({ left.x, left.y, halfWidth * 2.0f, bodyHeight }, fill);
    DrawTriangle(roof, left, right, fill);

    DrawLineEx(roof, left, thickness, WHITE);
    DrawLineEx(left, bottomLeft, thickness, WHITE);
    DrawLineEx(bottomLeft, bottomRight, thickness, WHITE);
    DrawLineEx(bottomRight, right, thickness, WHITE);
    DrawLineEx(right, roof, thickness, WHITE);
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
    // Stałe kamery i interfejsu użytkownika
    static constexpr float PAN_SPEED_BASE = 600.0f;
    static constexpr float ZOOM_SPEED_KBD = 2.0f;
    static constexpr float ZOOM_SPEED_MOUSE = 0.125f;
    static constexpr float ZOOM_MIN = 0.1f;
    static constexpr float ZOOM_MAX = 10.0f;
    static constexpr float GRID_STEP = 100.0f;
    static constexpr float UI_PADDING = 40.0f;
    static constexpr float BOTTOM_MENU_SPACE = 140.0f;
    static constexpr float BUTTON_WIDTH = 200.0f;

    Texture2D backgroundTexture;
    Rectangle viewportArea;
    Camera2D camera;
    Font uiFont;

    country* map_pointer;

    UI::Button btnBack;
    UI::Button btnLoad;
    UI::Slider sliderMapSize;

    int selectedItem = -1;
    int lastSelectedItem = -1;

    Visualization(country* map_pointer)
        : map_pointer(map_pointer), backgroundTexture{0}, uiFont{0} {
        camera = { 0 };
    }

    void Init() override {
        backgroundTexture = LoadTexture(UI::AssetPath(UI::GetBackgroundPath()));

        viewportArea = {
            UI_PADDING,
            UI_PADDING,
            (float)Config::SCREEN_WIDTH - (UI_PADDING * 2),
            (float)Config::SCREEN_HEIGHT - UI_PADDING - BOTTOM_MENU_SPACE
        };

        camera.target = { 0, 0 };
        camera.offset = { viewportArea.x + viewportArea.width / 2.0f, viewportArea.y + viewportArea.height / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        uiFont = UI::LoadStandardFont(40);

        btnBack = UI::Button({ viewportArea.x, viewportArea.y + viewportArea.height + 20.0f, BUTTON_WIDTH, 40 }, "WYJDŹ", uiFont, MAROON, RED);
        btnLoad = UI::Button({ viewportArea.x, btnBack.bounds.y + btnBack.bounds.height + 10.0f, BUTTON_WIDTH, 40 }, "ŁADUJ GRAF", uiFont);
        sliderMapSize = UI::Slider({ btnBack.bounds.x + 240.0f, btnBack.bounds.y + 15.0f, 300, 10 }, 500.0f, 10000.0f, Config::MAP_SIZE, GRID_STEP, "ROZMIAR MAPY:", uiFont, 20, 20, 30);
    }

    void Update(float deltatime, country* country_ptr) override {
        if (IsKeyReleased(KEY_ESCAPE)) {
            UI::PlaySelectSound();
            GoToTitle(map_pointer);
        }

        float panSpeed = PAN_SPEED_BASE * deltatime / camera.zoom;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    camera.target.y -= panSpeed;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  camera.target.y += panSpeed;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  camera.target.x -= panSpeed;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) camera.target.x += panSpeed;

        float keyboardZoomSpeed = ZOOM_SPEED_KBD * deltatime;
        if (IsKeyDown(KEY_KP_ADD) || IsKeyDown(KEY_EQUAL)) camera.zoom += keyboardZoomSpeed;
        if (IsKeyDown(KEY_KP_SUBTRACT) || IsKeyDown(KEY_MINUS)) camera.zoom -= keyboardZoomSpeed;

        camera.zoom = fminf(fmaxf(camera.zoom, ZOOM_MIN), ZOOM_MAX);

        if (CheckCollisionPointRec(GetMousePosition(), viewportArea)) {
            float wheel = GetMouseWheelMove();
            if (wheel != 0) {
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                camera.offset = GetMousePosition();
                camera.target = mouseWorldPos;
                camera.zoom += wheel * ZOOM_SPEED_MOUSE;
                camera.zoom = fminf(fmaxf(camera.zoom, ZOOM_MIN), ZOOM_MAX);
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                Vector2 delta = GetMouseDelta();
                camera.target.x -= delta.x / camera.zoom;
                camera.target.y -= delta.y / camera.zoom;
            }
        }

        // Clamp camera target to map dimensions
        camera.target.x = fminf(fmaxf(camera.target.x, -Config::MAP_HALF), Config::MAP_HALF);
        camera.target.y = fminf(fmaxf(camera.target.y, -Config::MAP_HALF), Config::MAP_HALF);

        bool backClicked = btnBack.Update();
        bool loadClicked = btnLoad.Update();
        bool sliderChanged = sliderMapSize.Update();

        if (btnBack.IsHovered()) selectedItem = 0;
        else if (btnLoad.IsHovered()) selectedItem = 1;
        else if (sliderMapSize.IsHovered()) selectedItem = 2;
        else selectedItem = -1;

        if (selectedItem != -1) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (selectedItem != lastSelectedItem) {
            if (selectedItem != -1) UI::PlayHoverSound();
            lastSelectedItem = selectedItem;
        }

        if (sliderChanged) {
            Config::MAP_SIZE = sliderMapSize.GetValue();
            Config::MAP_HALF = Config::MAP_SIZE / 2.0f;
        }

        if (backClicked) {
            UI::PlaySelectSound();
            GoToTitle(country_ptr);
        } else if (loadClicked) {
            UI::PlaySelectSound();
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(backgroundTexture);

        DrawRectangleRec(viewportArea, Fade(BLACK, 0.15f));

        BeginScissorMode((int)viewportArea.x, (int)viewportArea.y, (int)viewportArea.width, (int)viewportArea.height);
            BeginMode2D(camera);
                    for (int i = -Config::MAP_HALF; i <= Config::MAP_HALF; i += GRID_STEP) {
                        DrawLine(i, -Config::MAP_HALF, i, Config::MAP_HALF, Fade(RAYWHITE, 0.2f));
                        DrawLine(-Config::MAP_HALF, i, Config::MAP_HALF, i, Fade(RAYWHITE, 0.2f));
                    }

                    // Rysuj mapę jednostek kraju
                    DrawCountryEntities(map_pointer);
            EndMode2D();
        EndScissorMode();

        btnBack.Draw();
        btnLoad.Draw();
        sliderMapSize.Draw();

        if (selectedItem != -1) {
            Rectangle r = (selectedItem == 0) ? btnBack.bounds : (selectedItem == 1 ? btnLoad.bounds : sliderMapSize.track);
            DrawRectangleLinesEx({ r.x - 5, r.y - 5, r.width + 10, r.height + 10 }, 2, Color{96, 96, 0, 255});
        }

        DrawRectangleLinesEx(viewportArea, 3, RAYWHITE);
    }

    ~Visualization() {
        if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
        if (uiFont.texture.id > 0) UnloadFont(uiFont);
    }
};

#endif // STATE_VISUALIZATION_H
