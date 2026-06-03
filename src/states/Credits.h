#ifndef STATE_CREDITS_H
#define STATE_CREDITS_H

#include <src/backend/GameState.h>
#include <src/backend/StateManager.h>
#include <src/backend/UIHelpers.h>

inline void GoToTitle();

class Credits : public GameState {
    public:

    Texture2D marble_bg;
    Rectangle btnBack = { 0 };
    Font customFont;

    void Init() override {
        marble_bg = LoadTexture(UI::AssetPath("images/ui/marble.png"));
        customFont = UI::LoadStandardFont(40);
        btnBack = { 20, 20, 40, 40 };
    }

    void Update(float dt) override {
        Vector2 mouse = GetMousePosition();

        if (CheckCollisionPointRec(mouse, btnBack)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, btnBack)) {
                UI::PlaySelectSound();
                GoToTitle();
            }
        }

        if (IsKeyReleased(KEY_ESCAPE)) {
            UI::PlaySelectSound();
            GoToTitle();
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(marble_bg);

        const char* titleText = "TWÓRCY ROZWIĄZANIA PROJEKTU:";
        const char* names[] = {
            "Szymon Zakierski",
            "Antoni Narożny",
            "Dawid",
            "Franciszek"
        };

        Vector2 titleSize = MeasureTextEx(customFont, titleText, 40, 2);
        DrawTextEx(customFont, titleText, { (Config::SCREEN_WIDTH - titleSize.x) / 2, 100 }, 40, 2, DARKGRAY);

        for (int i = 0; i < 4; i++) {
            int posY = 220 + (i * 60);
            Vector2 nameSize = MeasureTextEx(customFont, names[i], 30, 2);
            DrawTextEx(customFont, names[i], { (Config::SCREEN_WIDTH - nameSize.x) / 2, (float)posY }, 30, 2, BLACK);
        }

        const char* audioCredits = "AUDIO: Freesound.org";
        Vector2 audioSize = MeasureTextEx(customFont, audioCredits, 15, 1);
        DrawTextEx(customFont, audioCredits, { (Config::SCREEN_WIDTH - audioSize.x) / 2, Config::SCREEN_HEIGHT - 40.0f }, 15, 1, DARKGRAY);

        bool hover = CheckCollisionPointRec(GetMousePosition(), btnBack);
        DrawRectangleRec(btnBack, hover ? RED : MAROON);
        DrawRectangleLinesEx(btnBack, 2, BLACK);
        DrawText("X", btnBack.x + 12, btnBack.y + 8, 25, WHITE);
    }

    ~Credits() {
        UnloadTexture(marble_bg);
        UnloadFont(customFont);
    }
};

#endif // STATE_CREDITS_H
