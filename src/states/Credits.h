#ifndef STATE_CREDITS_H
#define STATE_CREDITS_H

#include "../backend/GameState.h"
#include "../backend/StateManager.h"
#include "../backend/UIHelpers.h"
#include "../backend/classes/Button.h"

inline void GoToTitle(country* countryPtr);

class Credits : public GameState {
    public:
    // Stałe układu i stylizacji
    static constexpr float FOOTER_FONT_SIZE = 15.0f;

    static constexpr float TITLE_Y_OFFSET = 100.0f;
    static constexpr float NAMES_START_Y = 220.0f;
    static constexpr float NAMES_SPACING_Y = 60.0f;

    static constexpr float FOOTER_BOTTOM_MARGIN = 40.0f;

    Texture2D backgroundTexture;
    Font creditsFont;
    UI::Button btnBack;

    Credits() : backgroundTexture{0}, creditsFont{0} {}

    void Init() override {
        backgroundTexture = LoadTexture(UI::AssetPath(UI::GetBackgroundPath()));
        creditsFont = UI::LoadStandardFont(UI::HEADER_FONT_SIZE);

        btnBack = UI::Button({ UI::UI_MARGIN, UI::UI_MARGIN, UI::BACK_BTN_SIZE, UI::BACK_BTN_SIZE }, "X", creditsFont, MAROON, RED, WHITE, BLACK, UI::LABEL_FONT_SIZE, 0.0f);
    }

    void Update(float deltaTime, country* countryPtr) override {
        if (btnBack.IsHovered()) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (btnBack.Update()) {
            UI::PlaySelectSound();
            GoToTitle(countryPtr);
        }

        if (IsKeyReleased(KEY_ESCAPE)) {
            UI::PlaySelectSound();
            GoToTitle(countryPtr);
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(backgroundTexture);

        const char* titleText = "TWÓRCY ROZWIĄZANIA PROJEKTU:";
        const char* names[] = {
            "Antony Narożny",
            "Szymon Zakierski",
            "Dawid",
            "Franciszek"
        };

        Vector2 titleSize = MeasureTextEx(creditsFont, titleText, UI::HEADER_FONT_SIZE, UI::FONT_SPACING_HEADER);
        DrawTextEx(creditsFont, titleText, { (Config::SCREEN_WIDTH - titleSize.x) / 2, TITLE_Y_OFFSET }, UI::HEADER_FONT_SIZE, UI::FONT_SPACING_HEADER, DARKGRAY);

        for (size_t i = 0; i < sizeof(names)/sizeof(names[0]); i++) {
            float posY = NAMES_START_Y + (i * NAMES_SPACING_Y);
            Vector2 nameSize = MeasureTextEx(creditsFont, names[i], UI::SUBHEADER_FONT_SIZE, UI::FONT_SPACING_HEADER);
            DrawTextEx(creditsFont, names[i], { (Config::SCREEN_WIDTH - nameSize.x) / 2, posY }, UI::SUBHEADER_FONT_SIZE, UI::FONT_SPACING_HEADER, BLACK);
        }

        const char* audioCredits = "AUDIO: Freesound.org";
        Vector2 audioSize = MeasureTextEx(creditsFont, audioCredits, FOOTER_FONT_SIZE, UI::FONT_SPACING_NORMAL);
        DrawTextEx(creditsFont, audioCredits, { (Config::SCREEN_WIDTH - audioSize.x) / 2, (float)Config::SCREEN_HEIGHT - FOOTER_BOTTOM_MARGIN }, FOOTER_FONT_SIZE, UI::FONT_SPACING_NORMAL, DARKGRAY);

        btnBack.Draw();
    }

    ~Credits() {
        if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
        if (creditsFont.texture.id > 0) UnloadFont(creditsFont);
    }
};

#endif // STATE_CREDITS_H
