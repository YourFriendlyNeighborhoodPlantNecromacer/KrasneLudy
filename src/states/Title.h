#ifndef STATE_TITLE_H
#define STATE_TITLE_H

#include <cmath>
#include <src/backend/GameState.h>
#include <src/backend/StateManager.h>
#include <src/backend/UIHelpers.h>
#include "Visualization.h"
#include "Credits.h"

class Title : public GameState {
    public:

    Texture2D marble_bg;
    Texture2D main_logo;
    Texture2D umk_logo;
    Font customFont;

    float introTimer = 0.0f;
    const float fadeDuration = 2.0f;
    int selectedItem = 0;
    int lastSelectedItem = 0;

    Rectangle btnVis = { 0 };
    Rectangle btnCredits = { 0 };
    Rectangle btnExit = { 0 };
    Rectangle btnUmk = { 0 };

    void Init() override {
        marble_bg = LoadTexture(UI::AssetPath("images/ui/marble.png"));

        Image umk_img = LoadImage(UI::AssetPath("images/title/umk.png"));
        ImageResize(&umk_img, umk_img.width / 5, umk_img.height / 5);
        umk_logo = LoadTextureFromImage(umk_img);
        UnloadImage(umk_img);

        Image main_img = LoadImage(UI::AssetPath("images/title/logo.png"));
        ImageResize(&main_img, main_img.width * 1.5, main_img.height * 1.5);
        main_logo = LoadTextureFromImage(main_img);
        UnloadImage(main_img);

        customFont = UI::LoadStandardFont(40);

        float centerX = Config::SCREEN_WIDTH / 2.0f;
        btnVis = { centerX - 100, Config::SCREEN_HEIGHT / 2.0f + 50, 200, 40 };
        btnCredits = { centerX - 100, Config::SCREEN_HEIGHT / 2.0f + 110, 200, 40 };
        btnExit = { centerX - 100, Config::SCREEN_HEIGHT / 2.0f + 170, 200, 40 };
        btnUmk = { 5.0f, (float)Config::SCREEN_HEIGHT - umk_logo.height - 5, (float)umk_logo.width, (float)umk_logo.height };
    }

    void Update(float dt) override;

    void Draw() override {
        UI::DrawTiledBackground(marble_bg);

        float alpha = fminf(introTimer / fadeDuration, 1.0f);
        float posY = (Config::SCREEN_HEIGHT / 4.0f) * alpha;

        float swayX = sinf(GetTime() * 2.0f) * 15.0f;
        float centerX = (Config::SCREEN_WIDTH / 2.0f) - (main_logo.width / 2.0f);

        DrawTexture(main_logo, centerX + swayX, posY - 50, Fade(WHITE, alpha));

        UI::DrawMenuButton(btnVis, "WIZUALIZACJA", customFont);
        UI::DrawMenuButton(btnCredits, "TWÓRCY", customFont);
        UI::DrawMenuButton(btnExit, "WYJŚCIE", customFont);

        Rectangle r = (selectedItem == 0) ? btnVis : (selectedItem == 1 ? btnCredits : btnExit);
        DrawRectangleLinesEx({ r.x - 5, r.y - 5, r.width + 10, r.height + 10 }, 2, Color{(96),(96),(0),(255)});

        DrawTexture(umk_logo, 5, Config::SCREEN_HEIGHT - umk_logo.height - 5, WHITE);

        const char* yearText = "2026";
        Vector2 yearSize = MeasureTextEx(customFont, yearText, 20, 2);
        DrawTextEx(customFont, yearText, { (float)Config::SCREEN_WIDTH - yearSize.x - 10, (float)Config::SCREEN_HEIGHT - yearSize.y - 10 }, 20, 2, GRAY);
    }

    ~Title() {
        UnloadTexture(marble_bg);
        UnloadTexture(main_logo);
        UnloadTexture(umk_logo);
        UnloadFont(customFont);
    }
};

inline void Title::Update(float dt) {
    introTimer += dt;

    if (IsKeyPressed(KEY_UP)) selectedItem = (selectedItem + 2) % 3;
    if (IsKeyPressed(KEY_DOWN)) selectedItem = (selectedItem + 1) % 3;

    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, btnVis)) selectedItem = 0;
    else if (CheckCollisionPointRec(mouse, btnCredits)) selectedItem = 1;
    else if (CheckCollisionPointRec(mouse, btnExit)) selectedItem = 2;

    if (CheckCollisionPointRec(mouse, btnUmk) || CheckCollisionPointRec(mouse, btnVis) ||
        CheckCollisionPointRec(mouse, btnCredits) || CheckCollisionPointRec(mouse, btnExit)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (selectedItem != lastSelectedItem) {
        UI::PlayHoverSound();
        lastSelectedItem = selectedItem;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        UI::PlaySelectSound();
        if (selectedItem == 0) {
            StateManager::Instance().ChangeState(new Visualization());
        } else if (selectedItem == 1) {
            StateManager::Instance().ChangeState(new Credits());
        } else if (selectedItem == 2) {
            exit(0);
        }
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btnUmk)) {
            UI::PlaySelectSound();
            OpenURL("https://www.umk.pl/");
        } else if (CheckCollisionPointRec(mouse, btnVis)) {
            UI::PlaySelectSound();
            StateManager::Instance().ChangeState(new Visualization());
        } else if (CheckCollisionPointRec(mouse, btnCredits)) {
            UI::PlaySelectSound();
            StateManager::Instance().ChangeState(new Credits());
        } else if (CheckCollisionPointRec(mouse, btnExit)) {
            UI::PlaySelectSound();
            exit(0);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) exit(0);
}

inline void GoToTitle() {
    StateManager::Instance().ChangeState(new Title());
}

#endif // STATE_TITLE_H
