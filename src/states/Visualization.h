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
#include <vector>
#include <rlgl.h>

#include "../../DataStructures/country.h"

inline void GoToTitle(country* countryPtr);

static constexpr float MARKER_RADIUS = 32.0f;
static constexpr float MARKER_THICKNESS = 3.0f;

struct Road {
    Vector2 start;
    Vector2 end;
    Color color;
    int material;
};

struct RimPoint {
    Vector2 position;
    int material;
};

std::vector<Road> roadConnections;
std::vector<RimPoint> rimPoints;
std::vector<bool> worldMaterialFilter;

static inline void DrawWorkplaceMarker(float x, float y, Color fill) {
    Vector2 top = { x, y - MARKER_RADIUS };
    Vector2 right = { x + MARKER_RADIUS, y };
    Vector2 bottom = { x, y + MARKER_RADIUS };
    Vector2 left = { x - MARKER_RADIUS, y };

    DrawTriangle(top, bottom, right, fill);
    DrawTriangle(top, left, bottom, fill);

    DrawLineEx(top, right, MARKER_THICKNESS, BLACK);
    DrawLineEx(right, bottom, MARKER_THICKNESS, BLACK);
    DrawLineEx(bottom, left, MARKER_THICKNESS, BLACK);
    DrawLineEx(left, top, MARKER_THICKNESS, BLACK);
}

static constexpr float HOUSE_HALF_WIDTH = 32.0f;
static constexpr float HOUSE_BODY_HEIGHT = 28.0f;
static constexpr float HOUSE_ROOF_HEIGHT = 28.0f;

static inline void DrawHouseMarker(float x, float y, Color fill) {
    Vector2 left = { x - HOUSE_HALF_WIDTH, y };
    Vector2 right = { x + HOUSE_HALF_WIDTH, y };
    Vector2 roof = { x, y - HOUSE_ROOF_HEIGHT };
    Vector2 bottomLeft = { x - HOUSE_HALF_WIDTH, y + HOUSE_BODY_HEIGHT };
    Vector2 bottomRight = { x + HOUSE_HALF_WIDTH, y + HOUSE_BODY_HEIGHT };

    DrawRectangleRec({ left.x, left.y, HOUSE_HALF_WIDTH * 2.0f, HOUSE_BODY_HEIGHT }, fill);
    DrawTriangle(roof, left, right, fill);

    DrawLineEx(roof, left, MARKER_THICKNESS, WHITE);
    DrawLineEx(left, bottomLeft, MARKER_THICKNESS, WHITE);
    DrawLineEx(bottomLeft, bottomRight, MARKER_THICKNESS, WHITE);
    DrawLineEx(bottomRight, right, MARKER_THICKNESS, WHITE);
    DrawLineEx(right, roof, MARKER_THICKNESS, WHITE);
}

static inline void DrawCountryEntities(const country* c) {
    const auto& workplaces = c->getWorkplaces();
    const auto& houses = c->getHouses();

    for(int materialType = 0; materialType < namedValues::material::size; materialType++) {
        if (worldMaterialFilter[materialType]) {
            Color fill = Config::MATERIAL_COLORS.at(static_cast<namedValues::material>(materialType));

            for(int64_t i = 0; i <= workplaces[materialType].getLastIndex(); i++) {
                const auto& wp = workplaces[materialType][i];
                if(!wp) continue;
                DrawWorkplaceMarker((float)wp->coordinates[namedValues::axis::X] * Config::MAP_HALF, (float)wp->coordinates[namedValues::axis::Y] * Config::MAP_HALF, fill);
            }

            for(int64_t i = 0; i <= houses[materialType].getLastIndex(); i++) {
                const auto& housePtr = houses[materialType][i];
                if(!housePtr) continue;
                DrawHouseMarker((float)housePtr->coordinates[namedValues::axis::X] * Config::MAP_HALF, (float)housePtr->coordinates[namedValues::axis::Y] * Config::MAP_HALF, fill);
            }
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
    static constexpr float ROAD_OPTIONS_WIDTH = 160.0f;
    static constexpr float ROAD_OPTIONS_ITEM_HEIGHT = 30.0f;
    static constexpr float ROAD_OPTIONS_TOTAL_HEIGHT = ROAD_OPTIONS_ITEM_HEIGHT * namedValues::material::size;
    static constexpr float ROAD_THICKNESS = 8.0f;
    static constexpr float ROAD_OPTIONS_MAX_TEXT_WIDTH = ROAD_OPTIONS_WIDTH - 45.0f;

    // Układ i stałe interfejsu
    static constexpr float UI_BTN_HEIGHT = 40.0f;
    static constexpr float UI_BTN_OFFSET_Y = 20.0f;
    static constexpr float UI_BTN_SPACING_Y = 10.0f;
    static constexpr float UI_BTN_TOGGLE_WIDTH_DELTA = 60.0f;
    static constexpr float UI_BTN_OPTION_WIDTH = 55.0f;
    static constexpr float UI_BTN_WORLD_X_OFFSET = 240.0f;

    static constexpr float ROAD_OPTIONS_EXTRA_HEIGHT = 135.0f;
    static constexpr float WORLD_OPTIONS_HEIGHT = 240.0f;
    static constexpr float WORLD_OPTIONS_WIDTH = 160.0f;

    static constexpr float SLIDER_LABEL_Y = 15.0f;
    static constexpr float SLIDER_TRACK_Y = 45.0f;
    static constexpr float SLIDER_STEP_Y = 55.0f;
    static constexpr float SLIDER_WIDTH = 140.0f;
    static constexpr float SLIDER_HEIGHT = 10.0f;
    static constexpr float SLIDER_KNOB_SIZE = 15.0f;

    static constexpr float MENU_MARGIN = 5.0f;
    static constexpr float MENU_PADDING = 10.0f;
    static constexpr float MENU_BORDER_THICKNESS = 2.0f;
    static constexpr float MENU_DIVIDER_Y = 5.0f;
    static constexpr float MENU_ITEM_H = 30.0f;
    static constexpr float MENU_CIRCLE_R = 7.0f;
    static constexpr float MENU_TEXT_OFFSET_X = 30.0f;
    static constexpr float MENU_TEXT_OFFSET_Y = 6.0f;

    static constexpr float OVERLAY_ALPHA = 0.9f;
    static constexpr float VIEWPORT_DIM_ALPHA = 0.15f;
    static constexpr unsigned char SELECTOR_COLOR_R = 96;
    static constexpr unsigned char SELECTOR_COLOR_G = 96;

    Texture2D backgroundTexture;
    Rectangle viewportArea;
    Camera2D camera;
    Font uiFont;

    country* mapPointer;

    UI::Button btnBack;
    UI::Button btnToggleRoads;
    UI::Button btnToggleRim;
    UI::Button btnRoadRenderOptions;
    UI::Button btnWorldOptions;

    UI::Slider sliderRoadThickness;
    UI::Slider sliderRoadOpacity;
    UI::Slider sliderMapSize;
    UI::Slider sliderGridOpacity;
    UI::Slider sliderGridThickness;

    int selectedItem = -1;
    int lastSelectedItem = -1;
    int lastOverlayHoveredItem = -1;

    bool showRoads = false;
    bool showRim = false;
    bool showRoadOptions = false;
    bool showWorldOptions = false;
    bool gridEnabled = true;
    float roadThickness = 8.0f;
    float roadOpacity = 0.7f;
    float gridOpacity = 0.2f;
    float gridThickness = 1.0f;

    Visualization(country* mapPointer)
        : mapPointer(mapPointer), backgroundTexture{0}, uiFont{0} {
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

        uiFont = UI::LoadStandardFont((int)UI::HEADER_FONT_SIZE);

        btnBack = UI::Button({ viewportArea.x, viewportArea.y + viewportArea.height + UI_BTN_OFFSET_Y, UI::BUTTON_WIDTH, UI_BTN_HEIGHT }, "WYJDŹ", uiFont, MAROON, RED);
        btnToggleRoads = UI::Button({ viewportArea.x, btnBack.bounds.y + btnBack.bounds.height + UI_BTN_SPACING_Y, UI::BUTTON_WIDTH - UI_BTN_TOGGLE_WIDTH_DELTA, UI_BTN_HEIGHT }, "RYSUJ DROGI", uiFont);
        btnRoadRenderOptions = UI::Button({ btnToggleRoads.bounds.x + btnToggleRoads.bounds.width + MENU_MARGIN, btnToggleRoads.bounds.y, UI_BTN_OPTION_WIDTH, UI_BTN_HEIGHT }, "...", uiFont);
        btnWorldOptions = UI::Button({ btnBack.bounds.x + UI_BTN_WORLD_X_OFFSET, btnBack.bounds.y, UI::BUTTON_WIDTH, UI_BTN_HEIGHT }, "OPCJE ŚWIATA", uiFont);
        btnToggleRim = UI::Button({btnWorldOptions.bounds.x, btnToggleRoads.bounds.y, UI::BUTTON_WIDTH, UI::BUTTON_HEIGHT}, "RYSUJ OBWÓDKĘ", uiFont);

        sliderRoadThickness = UI::Slider({ 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT }, 1.0f, 30.0f, roadThickness, 1.0f, "", uiFont, 0, (int)SLIDER_KNOB_SIZE, 0);
        sliderRoadOpacity = UI::Slider({ 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT }, 0.0f, 1.0f, roadOpacity, 0.05f, "", uiFont, 0, (int)SLIDER_KNOB_SIZE, 0);

        sliderMapSize = UI::Slider({ 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT }, 400.0f, 10000.0f, Config::MAP_SIZE, GRID_STEP * 2.0f, "", uiFont, 0, (int)SLIDER_KNOB_SIZE, 0);
        sliderGridOpacity = UI::Slider({ 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT }, 0.0f, 1.0f, gridOpacity, 0.05f, "", uiFont, 0, (int)SLIDER_KNOB_SIZE, 0);
        sliderGridThickness = UI::Slider({ 0, 0, SLIDER_WIDTH, SLIDER_HEIGHT }, 0.1f, 10.0f, gridThickness, 0.5f, "", uiFont, 0, (int)SLIDER_KNOB_SIZE, 0);

        worldMaterialFilter.assign(namedValues::material::size, true);

        LoadAssignments();
        LoadRimPoints();
    }

    void LoadAssignments() {
        roadConnections.clear();
        std::ifstream file("dwarf_workplace_assignment_file_1.txt");
        if (!file.is_open()) return;

        std::string line;
        for (int m = 0; m < namedValues::material::size; ++m) {
            while (std::getline(file, line)) {
                if (line == ";;") break;
                if (line.empty()) continue;

                size_t sep = line.find(';');
                if (sep == std::string::npos) continue;

                int hIdx = std::stoi(line.substr(0, sep));
                int wIdx = std::stoi(line.substr(sep + 1));

                const auto& houses = mapPointer->getHouses()[m];
                const auto& workplaces = mapPointer->getWorkplaces()[m];

                if (hIdx <= houses.getLastIndex() && wIdx <= workplaces.getLastIndex()) {
                    auto& house = houses[hIdx];
                    auto& wp = workplaces[wIdx];
                    if (house && wp) {
                        Vector2 start = { (float)house->coordinates[namedValues::axis::X], (float)house->coordinates[namedValues::axis::Y] };
                        Vector2 end = { (float)wp->coordinates[namedValues::axis::X], (float)wp->coordinates[namedValues::axis::Y] };
                        Color color = Config::MATERIAL_COLORS.at(static_cast<namedValues::material>(m));
                        roadConnections.push_back({ start, end, color, m });
                    }
                }
            }
        }
    }

    void LoadRimPoints() {
        rimPoints.clear();
        std::ifstream file("rim_constructing_workplaces_file_1.txt");
        if (!file.is_open()) return;

        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            size_t sep = line.find(';');
            if (sep == std::string::npos) continue;

            int matIDX = std::stoi(line.substr(0, sep));
            int workIDX = std::stoi(line.substr(sep + 1));

            const auto& workplace = mapPointer->getWorkplaces()[matIDX][workIDX];
            if (workplace) {
                Vector2 pos = { (float)workplace->coordinates[namedValues::axis::X], (float)workplace->coordinates[namedValues::axis::Y] };
                rimPoints.push_back({ pos, matIDX });
            }
        }
    }

    void Update(float deltaTime, country* countryPtr) override {
        if (IsKeyReleased(KEY_ESCAPE)) {
            UI::PlaySelectSound();
            GoToTitle(mapPointer);
        }

        float panSpeed = PAN_SPEED_BASE * deltaTime / camera.zoom;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    camera.target.y -= panSpeed;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  camera.target.y += panSpeed;
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  camera.target.x -= panSpeed;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) camera.target.x += panSpeed;

        float keyboardZoomSpeed = ZOOM_SPEED_KBD * deltaTime;
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

        bool inputConsumedByMenu = false;
        Vector2 mouse = GetMousePosition();

        if (showRoadOptions) {
            float fullH = ROAD_OPTIONS_TOTAL_HEIGHT + ROAD_OPTIONS_EXTRA_HEIGHT;
            Vector2 menuPos = { btnRoadRenderOptions.bounds.x + btnRoadRenderOptions.bounds.width - ROAD_OPTIONS_WIDTH, btnRoadRenderOptions.bounds.y - fullH - UI_BTN_SPACING_Y };
            Rectangle menuArea = { menuPos.x - MENU_MARGIN, menuPos.y - MENU_MARGIN, ROAD_OPTIONS_WIDTH + (MENU_MARGIN * 2.0f), fullH + (MENU_MARGIN * 2.0f) };

            // Aktualizuj tory suwaków przed Update/Draw, aby zapewnić poprawną pozycję w pierwszej klatce
            sliderRoadThickness.track.x = menuPos.x + MENU_PADDING;
            sliderRoadThickness.track.y = menuPos.y + ROAD_OPTIONS_TOTAL_HEIGHT + SLIDER_TRACK_Y;
            sliderRoadOpacity.track.x = menuPos.x + MENU_PADDING;
            sliderRoadOpacity.track.y = sliderRoadThickness.track.y + SLIDER_STEP_Y;

            // Aktualizuj suwaki bez względu na najechanie, aby obsłużyć puszczenie przeciągania poza menu
            if (sliderRoadThickness.Update()) roadThickness = sliderRoadThickness.GetValue();
            if (sliderRoadOpacity.Update()) roadOpacity = sliderRoadOpacity.GetValue();

            if (CheckCollisionPointRec(mouse, menuArea)) {
                inputConsumedByMenu = true;

                // Logika dźwięku najechania dla elementów nakładki
                int currentHover = -1;
                for (int i = 0; i < namedValues::material::size; i++) {
                    if (CheckCollisionPointRec(mouse, { menuPos.x, menuPos.y + i * ROAD_OPTIONS_ITEM_HEIGHT, ROAD_OPTIONS_WIDTH, ROAD_OPTIONS_ITEM_HEIGHT }))
                        currentHover = i;
                }
                if (CheckCollisionPointRec(mouse, sliderRoadThickness.track)) currentHover = 10;
                if (CheckCollisionPointRec(mouse, sliderRoadOpacity.track)) currentHover = 11;

                if (currentHover != lastOverlayHoveredItem && currentHover != -1) {
                    UI::PlayHoverSound();
                }
                lastOverlayHoveredItem = currentHover;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    for (int i = 0; i < namedValues::material::size; i++) {
                        if (CheckCollisionPointRec(mouse, { menuPos.x, menuPos.y + i * ROAD_OPTIONS_ITEM_HEIGHT, ROAD_OPTIONS_WIDTH, ROAD_OPTIONS_ITEM_HEIGHT })) {
                            worldMaterialFilter[i] = !worldMaterialFilter[i];
                            UI::PlaySelectSound();
                            break;
                        }
                    }
                }
            } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(mouse, btnRoadRenderOptions.bounds)) {
                showRoadOptions = false;
            } else {
                lastOverlayHoveredItem = -1;
            }
        }

        if (showWorldOptions) {
            Vector2 menuPos = { btnWorldOptions.bounds.x, btnWorldOptions.bounds.y - WORLD_OPTIONS_HEIGHT - UI_BTN_SPACING_Y };
            Rectangle menuArea = { menuPos.x - MENU_MARGIN, menuPos.y - MENU_MARGIN, WORLD_OPTIONS_WIDTH + (MENU_MARGIN * 2.0f), WORLD_OPTIONS_HEIGHT + (MENU_MARGIN * 2.0f) };

            sliderMapSize.track.x = menuPos.x + MENU_PADDING;
            sliderMapSize.track.y = menuPos.y + SLIDER_TRACK_Y;

            float gridToggleY = menuPos.y + SLIDER_TRACK_Y + (SLIDER_STEP_Y / 2);
            float s2LabelY = gridToggleY + MENU_ITEM_H + MENU_MARGIN + (SLIDER_LABEL_Y / 3);

            sliderGridThickness.track.x = menuPos.x + MENU_PADDING;
            sliderGridThickness.track.y = s2LabelY + (SLIDER_TRACK_Y - SLIDER_LABEL_Y);

            sliderGridOpacity.track.x = menuPos.x + MENU_PADDING;
            sliderGridOpacity.track.y = sliderGridThickness.track.y + SLIDER_STEP_Y;

            if (sliderMapSize.Update()) { Config::MAP_SIZE = sliderMapSize.GetValue(); Config::MAP_HALF = Config::MAP_SIZE / 2.0f; }
            if (sliderGridThickness.Update()) gridThickness = sliderGridThickness.GetValue();
            if (sliderGridOpacity.Update()) gridOpacity = sliderGridOpacity.GetValue();

            if (CheckCollisionPointRec(mouse, menuArea)) {
                inputConsumedByMenu = true;

                int currentHover = -1;
                if (CheckCollisionPointRec(mouse, sliderMapSize.track)) currentHover = 0;
                Rectangle gridToggleRec = { menuPos.x, gridToggleY, WORLD_OPTIONS_WIDTH, MENU_ITEM_H };
                if (CheckCollisionPointRec(mouse, gridToggleRec)) currentHover = 1;
                if (CheckCollisionPointRec(mouse, sliderGridThickness.track)) currentHover = 2;
                if (CheckCollisionPointRec(mouse, sliderGridOpacity.track)) currentHover = 3;

                if (currentHover != lastOverlayHoveredItem && currentHover != -1) {
                    UI::PlayHoverSound();
                }
                lastOverlayHoveredItem = currentHover;

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentHover == 1) {
                    gridEnabled = !gridEnabled;
                    UI::PlaySelectSound();
                }
            } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(mouse, btnWorldOptions.bounds)) {
                showWorldOptions = false;
            } else {
                lastOverlayHoveredItem = -1;
            }
        }

        // Dynamiczna informacja zwrotna dla przycisków
        btnToggleRoads.hoverColor = showRoads ? DARKGREEN : DARKGRAY;
        btnToggleRoads.regularColor = showRoads ? Color{0, 160, 0, 255} : GRAY;

        btnToggleRim.hoverColor = showRim ? DARKGREEN : DARKGRAY;
        btnToggleRim.regularColor = showRim ? Color{0, 160, 0, 255} : GRAY;

        bool backClicked = false, roadsClicked = false, optionsClicked = false, worldClicked = false, rimClicked = false;
        if (!inputConsumedByMenu) {
            backClicked = btnBack.Update();
            roadsClicked = btnToggleRoads.Update();
            optionsClicked = btnRoadRenderOptions.Update();
            worldClicked = btnWorldOptions.Update();
            rimClicked = btnToggleRim.Update();

            if (btnBack.IsHovered()) selectedItem = 0;
            else if (btnToggleRoads.IsHovered()) selectedItem = 1;
            else if (btnRoadRenderOptions.IsHovered()) selectedItem = 2;
            else if (btnWorldOptions.IsHovered()) selectedItem = 3;
            else if (btnToggleRim.IsHovered()) selectedItem = 4;
            else selectedItem = -1;
        } else {
            selectedItem = -1;
        }

        if (selectedItem != -1) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (selectedItem != lastSelectedItem) {
            if (selectedItem != -1) UI::PlayHoverSound();
            lastSelectedItem = selectedItem;
        }

        if (backClicked) {
            UI::PlaySelectSound();
            GoToTitle(countryPtr);
        } else if (roadsClicked) {
            UI::PlaySelectSound();
            showRoads = !showRoads;
        } else if (optionsClicked) {
            UI::PlaySelectSound();
            showRoadOptions = !showRoadOptions;

            if(showRoadOptions)
                showWorldOptions = false;
        } else if (worldClicked) {
            UI::PlaySelectSound();
            showWorldOptions = !showWorldOptions;

            if(showWorldOptions)
                showRoadOptions = false;
        } else if (rimClicked) {
            UI::PlaySelectSound();
            showRim = !showRim;
        }
    }

    void DrawRoads() {
        for (const auto& road : roadConnections) {
            if (worldMaterialFilter[road.material]) {
                Vector2 s = { road.start.x * Config::MAP_HALF, road.start.y * Config::MAP_HALF };
                Vector2 e = { road.end.x * Config::MAP_HALF, road.end.y * Config::MAP_HALF };
                DrawLineEx(s, e, roadThickness, Fade(road.color, roadOpacity));
            }
        }
    }

    void DrawRim() {
        if (rimPoints.size() < 2) return;

        for (size_t i = 0; i < rimPoints.size(); ++i) {
            Vector2 p1_raw = rimPoints[i].position;
            Vector2 p2_raw;

            if (i == rimPoints.size() - 1) {
                p2_raw = rimPoints[0].position;
            } else {
                p2_raw = rimPoints[i+1].position;
            }

            Vector2 s = { p1_raw.x * Config::MAP_HALF, p1_raw.y * Config::MAP_HALF };
            Vector2 e = { p2_raw.x * Config::MAP_HALF, p2_raw.y * Config::MAP_HALF };

            DrawLineEx(s, e, ROAD_THICKNESS, Fade(Config::RIM_COLOR, roadOpacity));
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(backgroundTexture);

        DrawRectangleRec(viewportArea, Fade(BLACK, VIEWPORT_DIM_ALPHA));

        BeginScissorMode((int)viewportArea.x, (int)viewportArea.y, (int)viewportArea.width, (int)viewportArea.height);
            BeginMode2D(camera);
                if (gridEnabled) {
                    for (int i = -Config::MAP_HALF; i <= Config::MAP_HALF; i += GRID_STEP) {
                        DrawLineEx({(float)i, -Config::MAP_HALF}, {(float)i, Config::MAP_HALF}, gridThickness, Fade(RAYWHITE, gridOpacity));
                        DrawLineEx({-Config::MAP_HALF, (float)i}, {Config::MAP_HALF, (float)i}, gridThickness, Fade(RAYWHITE, gridOpacity));
                    }
                }

                // Rysuj drogi (połączenia) pod markerami dla lepszej czytelności
                if (showRoads) DrawRoads();
                if (showRim) DrawRim();

                // Rysuj mapę jednostek kraju
                DrawCountryEntities(mapPointer);
            EndMode2D();
        EndScissorMode();

        DrawRectangleLinesEx(viewportArea, MARKER_THICKNESS, RAYWHITE);

        btnBack.Draw();
        btnToggleRoads.Draw();
        btnToggleRim.Draw();
        btnRoadRenderOptions.Draw();
        btnWorldOptions.Draw();

        if (selectedItem != -1) {
            Rectangle r = (selectedItem == 0) ? btnBack.bounds :
                        (selectedItem == 1 ? btnToggleRoads.bounds :
                        (selectedItem == 2 ? btnRoadRenderOptions.bounds :
                        (selectedItem == 3 ? btnWorldOptions.bounds : btnToggleRim.bounds)));
            DrawRectangleLinesEx({ r.x - UI::SELECTOR_PADDING, r.y - UI::SELECTOR_PADDING, r.width + (UI::SELECTOR_PADDING * 2.0f), r.height + (UI::SELECTOR_PADDING * 2.0f) }, UI::SELECTOR_THICKNESS, Color{SELECTOR_COLOR_R, SELECTOR_COLOR_G, 0, 255});
        }

        if (showRoadOptions) DrawOptionsOverlay();
        if (showWorldOptions) DrawWorldOptionsOverlay();
    }

    void DrawOptionsOverlay() {
        float fullH = ROAD_OPTIONS_TOTAL_HEIGHT + ROAD_OPTIONS_EXTRA_HEIGHT;
        Vector2 pos = { btnRoadRenderOptions.bounds.x + btnRoadRenderOptions.bounds.width - ROAD_OPTIONS_WIDTH, btnRoadRenderOptions.bounds.y - fullH - UI_BTN_SPACING_Y };
        Vector2 mouse = GetMousePosition();

        DrawRectangleRec({ pos.x - MENU_MARGIN, pos.y - MENU_MARGIN, ROAD_OPTIONS_WIDTH + (MENU_MARGIN * 2.0f), fullH + (MENU_MARGIN * 2.0f) }, Fade(BLACK, OVERLAY_ALPHA));
        DrawRectangleLinesEx({ pos.x - MENU_MARGIN, pos.y - MENU_MARGIN, ROAD_OPTIONS_WIDTH + (MENU_MARGIN * 2.0f), fullH + (MENU_MARGIN * 2.0f) }, MENU_BORDER_THICKNESS, RAYWHITE);

        for (int i = 0; i < namedValues::material::size; i++) {
            Rectangle r = { pos.x, pos.y + i * ROAD_OPTIONS_ITEM_HEIGHT, ROAD_OPTIONS_WIDTH, ROAD_OPTIONS_ITEM_HEIGHT };
            bool isHovered = CheckCollisionPointRec(mouse, r);

            if (isHovered) {
                DrawRectangleRec(r, Fade(RAYWHITE, 0.2f));
            }

            Color mColor = Config::MATERIAL_COLORS.at(static_cast<namedValues::material>(i));
            const char* mName = Config::MATERIAL_NAMES.at(static_cast<namedValues::material>(i));
            Color textColor = worldMaterialFilter[i] ? (isHovered ? YELLOW : WHITE) : GRAY;

            DrawCircleV({ r.x + 15.0f, r.y + ROAD_OPTIONS_ITEM_HEIGHT / 2.0f }, MENU_CIRCLE_R, worldMaterialFilter[i] ? mColor : DARKGRAY);

            // Ściskanie tekstu jeżeli za dlugie
            Vector2 textSize = MeasureTextEx(uiFont, mName, UI::NORMAL_FONT_SIZE, 1.0f);
            float scaleX = (textSize.x > ROAD_OPTIONS_MAX_TEXT_WIDTH) ? (ROAD_OPTIONS_MAX_TEXT_WIDTH / textSize.x) : 1.0f;

            float textY = r.y + MENU_TEXT_OFFSET_Y;
            if (scaleX < 1.0f) {
                rlPushMatrix();
                rlTranslatef(r.x + MENU_TEXT_OFFSET_X, textY, 0.0f);
                rlScalef(scaleX, 1.0f, 1.0f);
                DrawTextEx(uiFont, mName, {0.0f, 0.0f}, UI::NORMAL_FONT_SIZE, 1.0f, textColor);
                rlPopMatrix();
            } else {
                DrawTextEx(uiFont, mName, { r.x + MENU_TEXT_OFFSET_X, textY }, UI::NORMAL_FONT_SIZE, 1.0f, textColor);
            }
        }

        float dividerY = pos.y + ROAD_OPTIONS_TOTAL_HEIGHT + MENU_DIVIDER_Y;
        DrawLine((int)pos.x + (int)MENU_MARGIN, (int)dividerY, (int)(pos.x + ROAD_OPTIONS_WIDTH - MENU_MARGIN), (int)dividerY, GRAY);

        DrawTextEx(uiFont, "GRUBOŚĆ DRÓG", { pos.x + MENU_PADDING, pos.y + ROAD_OPTIONS_TOTAL_HEIGHT + SLIDER_LABEL_Y }, UI::LIST_ITEM_FONT_SIZE, 1.0f, WHITE);
        sliderRoadThickness.Draw();
        DrawTextEx(uiFont, "PRZEŹROCZYSTOŚĆ", { pos.x + MENU_PADDING, pos.y + ROAD_OPTIONS_TOTAL_HEIGHT + SLIDER_LABEL_Y + SLIDER_STEP_Y }, UI::LIST_ITEM_FONT_SIZE, 1.0f, WHITE);
        sliderRoadOpacity.Draw();
    }

    void DrawWorldOptionsOverlay() {
        Vector2 pos = { btnWorldOptions.bounds.x, btnWorldOptions.bounds.y - WORLD_OPTIONS_HEIGHT - UI_BTN_SPACING_Y };
        Vector2 mouse = GetMousePosition();

        DrawRectangleRec({ pos.x - MENU_MARGIN, pos.y - MENU_MARGIN, WORLD_OPTIONS_WIDTH + (MENU_MARGIN * 2.0f), WORLD_OPTIONS_HEIGHT + (MENU_MARGIN * 2.0f) }, Fade(BLACK, OVERLAY_ALPHA));
        DrawRectangleLinesEx({ pos.x - MENU_MARGIN, pos.y - MENU_MARGIN, WORLD_OPTIONS_WIDTH + (MENU_MARGIN * 2.0f), WORLD_OPTIONS_HEIGHT + (MENU_MARGIN * 2.0f) }, MENU_BORDER_THICKNESS, RAYWHITE);

        DrawTextEx(uiFont, "ROZMIAR MAPY", { pos.x + MENU_PADDING, pos.y + SLIDER_LABEL_Y }, UI::LIST_ITEM_FONT_SIZE, 1.0f, WHITE);
        sliderMapSize.Draw();

        Rectangle gridToggleRec = { pos.x, pos.y + SLIDER_TRACK_Y + (SLIDER_STEP_Y / 2), WORLD_OPTIONS_WIDTH, MENU_ITEM_H };
        bool gridHover = CheckCollisionPointRec(mouse, gridToggleRec);
        if (gridHover) DrawRectangleRec(gridToggleRec, Fade(RAYWHITE, 0.2f));
        DrawCircleV({ pos.x + 15.0f, gridToggleRec.y + gridToggleRec.height / 2.0f }, MENU_CIRCLE_R, gridEnabled ? LIME : RED);
        DrawTextEx(uiFont, "POKAŻ SIATKĘ", { pos.x + MENU_TEXT_OFFSET_X, gridToggleRec.y + MENU_TEXT_OFFSET_Y }, UI::LIST_ITEM_FONT_SIZE, 1.0f, gridHover ? YELLOW : WHITE);

        float s2LabelY = gridToggleRec.y + gridToggleRec.height + MENU_MARGIN + (SLIDER_LABEL_Y / 3);
        DrawTextEx(uiFont, "GRUBOŚĆ SIATKI", { pos.x + MENU_PADDING, s2LabelY }, UI::LIST_ITEM_FONT_SIZE, 1.0f, WHITE);
        sliderGridThickness.Draw();
        DrawTextEx(uiFont, "WIDOCZNOŚĆ SIATKI", { pos.x + MENU_PADDING, s2LabelY + SLIDER_STEP_Y }, UI::LIST_ITEM_FONT_SIZE, 1.0f, WHITE);
        sliderGridOpacity.Draw();
    }

    ~Visualization() {
        if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
        if (uiFont.texture.id > 0) UnloadFont(uiFont);
    }
};

#endif // STATE_VISUALIZATION_H