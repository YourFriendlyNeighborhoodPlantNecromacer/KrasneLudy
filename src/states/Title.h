#ifndef STATE_TITLE_H
#define STATE_TITLE_H

#include <cmath>
#include "../backend/GameState.h"
#include "../backend/StateManager.h"
#include "../backend/UIHelpers.h"
#include "../backend/classes/Button.h"
#include "Visualization.h"
#include "Credits.h"
#include "Options.h"

class Title : public GameState {
    public:

    enum class MenuOption {
        VISUALIZATION,
        OPTIONS,
        CREDITS,
        EXIT,
        size
    };

    // Stałe układu i animacji
    static constexpr float FADE_DURATION = 2.0f;
    static constexpr float LOGO_SWAY_AMPLITUDE = 15.0f;
    static constexpr float LOGO_SWAY_SPEED = 2.0f;
    static constexpr float LOGO_SCALE = 1.5f;
    static constexpr float BUTTON_START_Y_OFFSET = 20.0f;
    static constexpr float BUTTON_SPACING = 50.0f;
    static constexpr float UI_MARGIN = 5.0f;

    static constexpr float LOGO_Y_OFFSET_DRAW = 50.0f;
    static constexpr float FOOTER_MARGIN = 10.0f;

    country* countryPtr;
    Texture2D backgroundTexture;
    Texture2D mainLogoTexture;
    Texture2D universityLogoTexture;
    Font menuFont;

    float introTimer = 0.0f;
    int selectedItem = 0;
    int lastSelectedItem = 0;

    UI::Button btnVis;
    UI::Button btnOptions;
    UI::Button btnCredits;
    UI::Button btnExit;
    UI::Button btnUmk;

    Title(country* countryPtr)
        : countryPtr(countryPtr), backgroundTexture{0}, mainLogoTexture{0},
          universityLogoTexture{0}, menuFont{0} {}

    void Init() override {
        backgroundTexture = LoadTexture(UI::AssetPath(UI::GetBackgroundPath()));

        Image umk_img = LoadImage(UI::AssetPath("images/title/umk.png"));
        ImageResize(&umk_img, umk_img.width / 5, umk_img.height / 5); // Zmień rozmiar o współczynnik
        universityLogoTexture = LoadTextureFromImage(umk_img);
        UnloadImage(umk_img);

        Image main_img = LoadImage(UI::AssetPath("images/title/logo.png"));
        ImageResizeNN(&main_img, (int)(main_img.width * LOGO_SCALE), (int)(main_img.height * LOGO_SCALE));
        mainLogoTexture = LoadTextureFromImage(main_img);
        SetTextureFilter(mainLogoTexture, TEXTURE_FILTER_POINT);
        UnloadImage(main_img);

        menuFont = UI::LoadStandardFont(UI::HEADER_FONT_SIZE);

        float centerX = Config::SCREEN_WIDTH / 2.0f;
        float buttonX = centerX - (UI::BUTTON_WIDTH / 2.0f);
        float startY = Config::SCREEN_HEIGHT / 2.0f + BUTTON_START_Y_OFFSET;

        btnVis     = UI::Button({ buttonX, startY, UI::BUTTON_WIDTH, UI::BUTTON_HEIGHT }, "WIZUALIZACJA", menuFont);
        btnOptions = UI::Button({ buttonX, startY + (1 * BUTTON_SPACING), UI::BUTTON_WIDTH, UI::BUTTON_HEIGHT }, "OPCJE", menuFont);
        btnCredits = UI::Button({ buttonX, startY + (2 * BUTTON_SPACING), UI::BUTTON_WIDTH, UI::BUTTON_HEIGHT }, "TWÓRCY", menuFont);
        btnExit    = UI::Button({ buttonX, startY + (3 * BUTTON_SPACING), UI::BUTTON_WIDTH, UI::BUTTON_HEIGHT }, "WYJŚCIE", menuFont);

        btnUmk = UI::Button({ UI_MARGIN, (float)Config::SCREEN_HEIGHT - universityLogoTexture.height - UI_MARGIN, (float)universityLogoTexture.width, (float)universityLogoTexture.height }, "", menuFont, BLANK, BLANK, WHITE, BLANK, 20, 0.0f);
    }

    void Update(float deltaTime, country* countryPtr) override;

    void ExecuteMenuAction(int index, country* countryPtr) {
        UI::PlaySelectSound();
        switch (static_cast<MenuOption>(index)) {
            case MenuOption::VISUALIZATION: StateManager::Instance().ChangeState(new Visualization(countryPtr)); break;
            case MenuOption::OPTIONS:       StateManager::Instance().ChangeState(new Options()); break;
            case MenuOption::CREDITS:       StateManager::Instance().ChangeState(new Credits()); break;
            case MenuOption::EXIT:          exit(0); break;
            default: break;
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(backgroundTexture);

        float alpha = fminf(introTimer / FADE_DURATION, 1.0f);
        float posY = (Config::SCREEN_HEIGHT / 6.0f) * alpha;
        float swayX = sinf(GetTime() * LOGO_SWAY_SPEED) * LOGO_SWAY_AMPLITUDE;
        float centerX = (Config::SCREEN_WIDTH / 2.0f) - (mainLogoTexture.width / 2.0f);

        DrawTexture(mainLogoTexture, centerX + swayX, posY - LOGO_Y_OFFSET_DRAW, Fade(WHITE, alpha));

        btnVis.Draw();
        btnOptions.Draw();
        btnCredits.Draw();
        btnExit.Draw();

        Rectangle selectionRect;

        if (selectedItem == 0) selectionRect = btnVis.bounds;
        else if (selectedItem == 1) selectionRect = btnOptions.bounds;
        else if (selectedItem == 2) selectionRect = btnCredits.bounds;
        else selectionRect = btnExit.bounds;

        DrawRectangleLinesEx({ selectionRect.x - UI::SELECTOR_PADDING, selectionRect.y - UI::SELECTOR_PADDING, selectionRect.width + UI::SELECTOR_PADDING * 2.0f, selectionRect.height + UI::SELECTOR_PADDING * 2.0f }, 2, Color{96, 96, 0, 255});

        DrawTexture(universityLogoTexture, UI_MARGIN, Config::SCREEN_HEIGHT - universityLogoTexture.height - UI_MARGIN, WHITE);

        const char* yearText = "2026";
        Vector2 yearSize = MeasureTextEx(menuFont, yearText, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_HEADER);
        DrawTextEx(menuFont, yearText, { (float)Config::SCREEN_WIDTH - yearSize.x - FOOTER_MARGIN, (float)Config::SCREEN_HEIGHT - yearSize.y - FOOTER_MARGIN }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_HEADER, GRAY);
    }

    ~Title() {
        if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
        if (mainLogoTexture.id > 0) UnloadTexture(mainLogoTexture);
        if (universityLogoTexture.id > 0) UnloadTexture(universityLogoTexture);
        if (menuFont.texture.id > 0) UnloadFont(menuFont);
    }
};

inline void Title::Update(float deltaTime, country* countryPtr) {
    introTimer += deltaTime;
    int totalItems = static_cast<int>(MenuOption::size);

    if (IsKeyPressed(KEY_UP)) selectedItem = (selectedItem + totalItems - 1) % totalItems;
    if (IsKeyPressed(KEY_DOWN)) selectedItem = (selectedItem + 1) % totalItems;

    // Aktualizuj przyciski i śledź kliknięcia
    bool clicked[] = { btnVis.Update(), btnOptions.Update(), btnCredits.Update(), btnExit.Update() };
    bool umkClicked = btnUmk.Update();

    // Synchronizuj wybór z najechaniem myszą
    if      (btnVis.IsHovered())     selectedItem = static_cast<int>(MenuOption::VISUALIZATION);
    else if (btnOptions.IsHovered()) selectedItem = static_cast<int>(MenuOption::OPTIONS);
    else if (btnCredits.IsHovered()) selectedItem = static_cast<int>(MenuOption::CREDITS);
    else if (btnExit.IsHovered())    selectedItem = static_cast<int>(MenuOption::EXIT);

    // Informacja zwrotna kursora
    if (btnUmk.IsHovered() || btnVis.IsHovered() || btnOptions.IsHovered() || btnCredits.IsHovered() || btnExit.IsHovered()) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    } else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    // Dźwięk wyboru
    if (selectedItem != lastSelectedItem) {
        UI::PlayHoverSound();
        lastSelectedItem = selectedItem;
    }

    // Skrócone wykonanie akcji
    if (IsKeyPressed(KEY_ENTER)) {
        ExecuteMenuAction(selectedItem, countryPtr);
    } else if (umkClicked) {
        UI::PlaySelectSound();
        OpenURL("https://www.umk.pl/");
    } else {
        for (int i = 0; i < totalItems; i++) {
            if (clicked[i]) ExecuteMenuAction(i, countryPtr);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) exit(0);
}

inline void GoToTitle(country* countryPtr) {
    StateManager::Instance().ChangeState(new Title(countryPtr));
}

#endif // STATE_TITLE_H
