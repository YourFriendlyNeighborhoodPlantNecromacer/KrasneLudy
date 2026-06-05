#ifndef STATE_OPTIONS_H
#define STATE_OPTIONS_H

#include <cmath>
#include "../backend/GameState.h"
#include "../backend/StateManager.h"
#include "../backend/SettingsManager.h"
#include <vector>
#include "../backend/UIHelpers.h"
#include "../backend/classes/Button.h"

// Deklaracja wyprzedzająca
inline void GoToTitle(country* country_ptr);

class Options : public GameState {
public:
    // Stałe układu
    static constexpr float SECTION_OFFSET_X = 50.0f;
    static constexpr float AUDIO_SECTION_OFFSET_X = 400.0f;
    static constexpr float DISPLAY_SECTION_Y = 80.0f;
    static constexpr float MISC_SECTION_Y = 360.0f;
    static constexpr float LIST_START_Y_OFFSET = 45.0f;
    static constexpr float ITEM_HEIGHT_SPACING = 45.0f;
    static constexpr float BUTTON_WIDTH = 320.0f;
    static constexpr float BUTTON_HEIGHT = 35.0f;
    static constexpr float AUDIO_BTN_SIZE = 35.0f;
    static constexpr float VOLUME_STEP = 0.1f;

    // Zakresy indeksów wyboru
    static constexpr int IDX_DISPLAY_START = 0;
    static constexpr int DISPLAY_OPTIONS_COUNT = 3;

    static constexpr int IDX_AUDIO_START = 3;
    static constexpr int AUDIO_OPTIONS_COUNT = 3;

    static constexpr int IDX_MISC_START = 6;
    static constexpr int MISC_OPTIONS_COUNT = 2;

    // Stylizacja interfejsu użytkownika
    static constexpr float HEADER_FONT_SIZE = 30.0f;
    static constexpr float NORMAL_FONT_SIZE = 20.0f;

    Texture2D backgroundTexture;
    Font headerFont;
    Font normalFont;

    int selectedItem = 0;
    int lastSelectedItem = -1;

    UI::Button btnBack;
    UI::Button btnReset;
    UI::Button btnResetConfirmYes;
    UI::Button btnResetConfirmNo;
    bool showResetConfirmDialog = false;

    // Grupowane przyciski posekcjach
    std::vector<UI::Button> displayButtons;
    std::vector<UI::Button> audioButtons; // Pary + i - przycisków
    std::vector<UI::Button> miscButtons;

    Options() : backgroundTexture{0}, headerFont{0}, normalFont{0} {}

    void Init() override {
        backgroundTexture = LoadTexture(UI::AssetPath(UI::GetBackgroundPath()));
        headerFont = UI::LoadStandardFont(HEADER_FONT_SIZE);
        normalFont = UI::LoadStandardFont(NORMAL_FONT_SIZE);

        // Ładuj z pliku
        SettingsManager::Instance().LoadSettings();

        btnBack = UI::Button({20, 20, 40, 40}, "X", headerFont, MAROON, RED, WHITE, BLACK, 25, 0.0f);
        btnReset = UI::Button({Config::SCREEN_WIDTH - 220, 20, 200, 40}, "RESETUJ USTAWIENIA", normalFont);
        btnResetConfirmYes = UI::Button({Config::SCREEN_WIDTH / 2 - 120, Config::SCREEN_HEIGHT / 2 + 40, 100, 40}, "TAK", headerFont);
        btnResetConfirmNo = UI::Button({Config::SCREEN_WIDTH / 2 + 20, Config::SCREEN_HEIGHT / 2 + 40, 100, 40}, "NIE", headerFont);

        // Powstawianie opcji dla różnych sekcji
        for (int i = 0; i < DISPLAY_OPTIONS_COUNT; i++)
            displayButtons.emplace_back(Rectangle{SECTION_OFFSET_X, (DISPLAY_SECTION_Y + LIST_START_Y_OFFSET) + (i * ITEM_HEIGHT_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT}, "", normalFont);

        for (int i = 0; i < AUDIO_OPTIONS_COUNT; i++) {
            float y = (DISPLAY_SECTION_Y + LIST_START_Y_OFFSET) + (i * ITEM_HEIGHT_SPACING);
            audioButtons.emplace_back(Rectangle{AUDIO_SECTION_OFFSET_X + 165, y, AUDIO_BTN_SIZE, AUDIO_BTN_SIZE}, "-", normalFont, DARKBLUE, BLUE, WHITE, BLACK, 20, 0.0f);
            audioButtons.emplace_back(Rectangle{AUDIO_SECTION_OFFSET_X + 315, y, AUDIO_BTN_SIZE, AUDIO_BTN_SIZE}, "+", normalFont, DARKBLUE, BLUE, WHITE, BLACK, 20, 0.0f);
        }

        for (int i = 0; i < MISC_OPTIONS_COUNT; i++)
            miscButtons.emplace_back(Rectangle{SECTION_OFFSET_X, (MISC_SECTION_Y + LIST_START_Y_OFFSET) + (i * ITEM_HEIGHT_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT}, "", normalFont);
    }

    int GetItemCount() const {
        if (showResetConfirmDialog) return 0; // Dialog blokuje nawigację
        return displayButtons.size() + (audioButtons.size() / 2) + miscButtons.size();
    }

    void SaveSettings() {
        GameSettings& settings = SettingsManager::Instance().GetSettingsMutable();
        settings.ApplyToGame();
        SettingsManager::Instance().SaveSettings();
    }

    void Update(float deltatime, country* country_ptr) override {
        // Dialog resetowanie ustawień
        if (showResetConfirmDialog) {
            if (btnResetConfirmYes.Update() || IsKeyPressed(KEY_Y)) {
                UI::PlaySelectSound();
                SettingsManager::Instance().ResetToDefaults();
                SettingsManager::Instance().GetSettings().ApplyToGame();

                // Przeładuj teksturę tła, aby odświeżyć widok menu
                UnloadTexture(backgroundTexture);
                backgroundTexture = LoadTexture(UI::AssetPath(UI::GetBackgroundPath()));

                showResetConfirmDialog = false;
            }
            if (btnResetConfirmNo.Update() || IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE)) {
                UI::PlaySelectSound();
                showResetConfirmDialog = false;
            }
            return;
        }

        // Powrót
        if (btnBack.Update() || IsKeyReleased(KEY_ESCAPE)) {
            UI::PlaySelectSound();
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            GoToTitle(country_ptr);
            return;
        }

        // Reset Ustawień
        if (btnReset.Update()) {
            UI::PlaySelectSound();
            showResetConfirmDialog = true;
            return;
        }

        SetMouseCursor(btnBack.IsHovered() || btnReset.IsHovered() ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

        int itemCount = GetItemCount();
        if (itemCount > 0) {
            if (IsKeyPressed(KEY_UP)) selectedItem = (selectedItem + itemCount - 1) % itemCount;
            if (IsKeyPressed(KEY_DOWN)) selectedItem = (selectedItem + 1) % itemCount;
        }

        UpdateDisplayOptions();
        UpdateAudioOptions();
        UpdateMiscOptions();

        if (selectedItem != lastSelectedItem) {
            UI::PlayHoverSound();
            lastSelectedItem = selectedItem;
        }
    }

    // Pomocnicze - Opcje przyciskowe
    bool HandleToggle(UI::Button& btn, int index, bool& setting) {
        if (btn.IsHovered()) selectedItem = index;
        if (btn.Update() || (selectedItem == index && IsKeyPressed(KEY_ENTER))) {
            UI::PlaySelectSound();
            setting = !setting;
            SaveSettings();
            return true;
        }
        return false;
    }

    void UpdateDisplayOptions() {
        GameSettings& settings = SettingsManager::Instance().GetSettingsMutable();
        HandleToggle(displayButtons[0], IDX_DISPLAY_START, settings.fullscreen);
        HandleToggle(displayButtons[1], IDX_DISPLAY_START + 1, settings.vsync);

        // Opcja Framerate - wyłączona gdy VSync jest ON
        if (!settings.vsync) {
            if (displayButtons[2].IsHovered()) selectedItem = IDX_DISPLAY_START + 2;
            if (displayButtons[2].Update() || (selectedItem == IDX_DISPLAY_START + 2 && IsKeyPressed(KEY_ENTER))) {
                int fpsLevels[] = { 30, 60, 75, 120, 144, 240 };
                int currentIdx = 1; // domyślnie 60
                for(int i=0; i<6; i++) if(fpsLevels[i] == settings.targetFPS) currentIdx = i;
                settings.targetFPS = fpsLevels[(currentIdx + 1) % 6];
                SaveSettings();
            }
        }
    }

    void UpdateAudioOptions() {
        GameSettings& settings = SettingsManager::Instance().GetSettingsMutable();
        float* vals[] = { &settings.masterVolume, &settings.musicVolume, &settings.sfxVolume };
        for (int i = 0; i < AUDIO_OPTIONS_COUNT; i++) {
            int globalIdx = i + IDX_AUDIO_START;
            if (audioButtons[i * 2].IsHovered() || audioButtons[i * 2 + 1].IsHovered()) selectedItem = globalIdx;
            if (audioButtons[i * 2].Update() || (selectedItem == globalIdx && IsKeyPressed(KEY_LEFT))) {
                *vals[i] = fmaxf(0.0f, *vals[i] - VOLUME_STEP);
                UI::PlaySelectSound();
                SaveSettings();
            }
            if (audioButtons[i * 2 + 1].Update() || (selectedItem == globalIdx && IsKeyPressed(KEY_RIGHT))) {
                *vals[i] = fminf(1.0f, *vals[i] + VOLUME_STEP);
                UI::PlaySelectSound();
                SaveSettings();
            }
        }
    }

    void UpdateMiscOptions() {
        GameSettings& settings = SettingsManager::Instance().GetSettingsMutable();
        HandleToggle(miscButtons[0], IDX_MISC_START, settings.showFPS);
        if (miscButtons[1].IsHovered()) selectedItem = IDX_MISC_START + 1;
        if (miscButtons[1].Update() || (selectedItem == IDX_MISC_START + 1 && IsKeyPressed(KEY_ENTER))) {
            settings.menuBackgroundIndex = (settings.menuBackgroundIndex + 1) % 2;
            SaveSettings();
            // Przeładuj teksturę tła
            UnloadTexture(backgroundTexture);
            backgroundTexture = LoadTexture(UI::AssetPath(UI::GetBackgroundPath()));
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(backgroundTexture);
        const char* titleText = "OPCJE APLIKACJI";
        DrawTextEx(headerFont, titleText, {(Config::SCREEN_WIDTH - MeasureTextEx(headerFont, titleText, HEADER_FONT_SIZE, 2).x) / 2, 20}, HEADER_FONT_SIZE, 2, DARKGRAY);

        if (showResetConfirmDialog) {
            DrawResetConfirmDialog();
        } else {
            DrawDisplaySection();
            DrawAudioSection();
            DrawMiscSection();
            DrawSelector();
            btnBack.Draw();
            btnReset.Draw();
        }
    }

    void DrawResetConfirmDialog() {
        DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Color{0, 0, 0, 150});

        float dialogWidth = 400;
        float dialogHeight = 200;
        float dialogX = (Config::SCREEN_WIDTH - dialogWidth) / 2;
        float dialogY = (Config::SCREEN_HEIGHT - dialogHeight) / 2;

        DrawRectangleRec({dialogX, dialogY, dialogWidth, dialogHeight}, DARKGRAY);
        DrawRectangleLinesEx({dialogX, dialogY, dialogWidth, dialogHeight}, 2, GOLD);

        const char* titleText = "RESETOWAĆ USTAWIENIA?";
        Vector2 titleSize = MeasureTextEx(headerFont, titleText, 24, 1);
        DrawTextEx(headerFont, titleText, {dialogX + (dialogWidth - titleSize.x) / 2, dialogY + 30}, 24, 1, WHITE);

        const char* msgText = "Wszystkie ustawienia zostaną przywrócone\ndo wartości domyślnych.";
        DrawTextEx(normalFont, msgText, {dialogX + 20, dialogY + 70}, 16, 1, RAYWHITE);

        btnResetConfirmYes.Draw();
        btnResetConfirmNo.Draw();
    }

    void DrawDisplaySection() {
        const GameSettings& settings = SettingsManager::Instance().GetSettings();
        float startX = SECTION_OFFSET_X, startY = DISPLAY_SECTION_Y;
        bool isSectionActive = (selectedItem >= IDX_DISPLAY_START && selectedItem < IDX_DISPLAY_START + DISPLAY_OPTIONS_COUNT);
        DrawTextEx(headerFont, "USTAWIENIA WYŚWIETLANIA:", {startX, startY}, 20, 1, isSectionActive ? YELLOW : DARKGRAY);

        const char* labels[] = {"Tryb Okna", "VSync", "Limit FPS"};

        std::string fpsVal = settings.vsync ? "AUTO (VSYNC)" : std::to_string(settings.targetFPS);
        const char* values[] = {
            settings.fullscreen ? "PEŁNY EKRAN" : "OKNO",
            settings.vsync ? "WŁ." : "WYŁ.",
            fpsVal.c_str()
        };

        for (int i = 0; i < DISPLAY_OPTIONS_COUNT; i++) {
            // Wyszarzenie jeśli VSync jest włączony dla opcji FPS (indeks 2)
            bool isGrayed = (settings.vsync && i == 2);
            Color itemColor = isGrayed ? Color{64, 64, 64, 255} : ((selectedItem == i) ? RAYWHITE : BLACK);

            if (!isGrayed) displayButtons[i].Draw();
            else DrawRectangleRec(displayButtons[i].bounds, Color{40, 40, 40, 100}); // Tło dla zablokowanego przycisku

            DrawTextEx(normalFont, TextFormat("%s: %s", labels[i], values[i]), {startX + 10, (startY + LIST_START_Y_OFFSET) + (i * ITEM_HEIGHT_SPACING) + 8}, 18, 1, itemColor);
        }
    }

    void DrawAudioSection() {
        const GameSettings& settings = SettingsManager::Instance().GetSettings();
        float startX = AUDIO_SECTION_OFFSET_X, startY = DISPLAY_SECTION_Y;
        bool isSectionActive = (selectedItem >= IDX_AUDIO_START && selectedItem < IDX_AUDIO_START + AUDIO_OPTIONS_COUNT);
        DrawTextEx(headerFont, "USTAWIENIA AUDIO:", {startX, startY}, 20, 1, isSectionActive ? YELLOW : DARKGRAY);

        const char* labels[] = {"Główna Głośność", "Głośność Muzyki", "Głośność Dźwięków"};
        float values[] = { settings.masterVolume, settings.musicVolume, settings.sfxVolume };

        for (int i = 0; i < AUDIO_OPTIONS_COUNT; i++) {
            float rowY = (startY + LIST_START_Y_OFFSET) + (i * ITEM_HEIGHT_SPACING);
            int globalIdx = i + IDX_AUDIO_START;
            DrawTextEx(normalFont, TextFormat("%s: %.0f%%", labels[i], values[i] * 100), {startX, rowY + 8}, 18, 1, (selectedItem == globalIdx) ? RAYWHITE : BLACK);
            DrawVolumeBar(startX + 205, rowY + 10, values[i]);
            audioButtons[i * 2].Draw();
            audioButtons[i * 2 + 1].Draw();
        }
    }

    void DrawMiscSection() {
        const GameSettings& settings = SettingsManager::Instance().GetSettings();
        float startX = SECTION_OFFSET_X, startY = MISC_SECTION_Y;
        bool isSectionActive = (selectedItem >= IDX_MISC_START && selectedItem < IDX_MISC_START + MISC_OPTIONS_COUNT);
        DrawTextEx(headerFont, "INNE USTAWIENIA:", {startX, startY}, 20, 1, isSectionActive ? YELLOW : DARKGRAY);

        const char* labels[] = {"Wyświetlaj FPS", "Tło Menu"};
        const char* bgTexts[] = {"Marmur", "Ciemne"};
        const char* values[] = { settings.showFPS ? "WŁ." : "WYŁ.", bgTexts[settings.menuBackgroundIndex] };

        for (int i = 0; i < MISC_OPTIONS_COUNT; i++) {
            int globalIdx = i + IDX_MISC_START;
            miscButtons[i].Draw();
            DrawTextEx(normalFont, TextFormat("%s: %s", labels[i], values[i]), {startX + 10, (startY + LIST_START_Y_OFFSET) + (i * ITEM_HEIGHT_SPACING) + 8}, 18, 1, (selectedItem == globalIdx) ? RAYWHITE : BLACK);
        }
    }

    void DrawSelector() {
        Rectangle r = {0};
        if (selectedItem >= IDX_DISPLAY_START && selectedItem < IDX_DISPLAY_START + DISPLAY_OPTIONS_COUNT) r = displayButtons[selectedItem].bounds;
        else if (selectedItem >= IDX_AUDIO_START && selectedItem < IDX_AUDIO_START + AUDIO_OPTIONS_COUNT) r = { audioButtons[(selectedItem - IDX_AUDIO_START) * 2].bounds.x - 165, audioButtons[(selectedItem - IDX_AUDIO_START) * 2].bounds.y, 355, BUTTON_HEIGHT };
        else if (selectedItem >= IDX_MISC_START && selectedItem < IDX_MISC_START + MISC_OPTIONS_COUNT) r = miscButtons[selectedItem - IDX_MISC_START].bounds;

        if (r.width > 0) DrawRectangleLinesEx({r.x - 5, r.y - 5, r.width + 10, r.height + 10}, 2, Color{96, 96, 0, 255});
    }

    void DrawVolumeBar(float x, float y, float volume) {
        float barWidth = 100;
        float barHeight = 15;
        DrawRectangleLines((int)x, (int)y, (int)barWidth, (int)barHeight, BLACK);
        DrawRectangle((int)x, (int)y, (int)(barWidth * volume), (int)barHeight, DARKBLUE);
    }

    ~Options() {
        if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
        if (headerFont.texture.id > 0) UnloadFont(headerFont);
        if (normalFont.texture.id > 0) UnloadFont(normalFont);
    }
};

inline void GoToOptions(country* country_ptr) {
    StateManager::Instance().ChangeState(new Options());
}

#endif // STATE_OPTIONS_H
