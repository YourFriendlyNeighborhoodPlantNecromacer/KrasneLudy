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
    static constexpr float BUTTON_WIDTH = 200.0f;
    static constexpr float BUTTON_HEIGHT = 40.0f;
    static constexpr float BUTTON_START_Y_OFFSET = 20.0f;
    static constexpr float BUTTON_SPACING = 50.0f;
    static constexpr float UI_MARGIN = 5.0f;
    static constexpr float FOOTER_TEXT_SIZE = 20.0f;
    static constexpr float MENU_FONT_SIZE = 40.0f;

    country* country_ptr;
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

    Title(country* country_ptr)
        : country_ptr(country_ptr), backgroundTexture{0}, mainLogoTexture{0},
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

        menuFont = UI::LoadStandardFont(MENU_FONT_SIZE);

        float centerX = Config::SCREEN_WIDTH / 2.0f;
        float buttonX = centerX - (BUTTON_WIDTH / 2.0f);
        float startY = Config::SCREEN_HEIGHT / 2.0f + BUTTON_START_Y_OFFSET;

        btnVis     = UI::Button({ buttonX, startY, BUTTON_WIDTH, BUTTON_HEIGHT }, "WIZUALIZACJA", menuFont);
        btnOptions = UI::Button({ buttonX, startY + (1 * BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT }, "OPCJE", menuFont);
        btnCredits = UI::Button({ buttonX, startY + (2 * BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT }, "TWÓRCY", menuFont);
        btnExit    = UI::Button({ buttonX, startY + (3 * BUTTON_SPACING), BUTTON_WIDTH, BUTTON_HEIGHT }, "WYJŚCIE", menuFont);

        btnUmk = UI::Button({ UI_MARGIN, (float)Config::SCREEN_HEIGHT - universityLogoTexture.height - UI_MARGIN, (float)universityLogoTexture.width, (float)universityLogoTexture.height }, "", menuFont, BLANK, BLANK, WHITE, BLANK, 20, 0.0f);
    }

    void Update(float deltatime, country* country_ptr) override;

    void ExecuteMenuAction(int index, country* country_ptr) {
        UI::PlaySelectSound();
        switch (static_cast<MenuOption>(index)) {
            case MenuOption::VISUALIZATION: StateManager::Instance().ChangeState(new Visualization(country_ptr)); break;
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

        DrawTexture(mainLogoTexture, centerX + swayX, posY - 50, Fade(WHITE, alpha));

        btnVis.Draw();
        btnOptions.Draw();
        btnCredits.Draw();
        btnExit.Draw();

        Rectangle selectionRect;

        if (selectedItem == 0) selectionRect = btnVis.bounds;
        else if (selectedItem == 1) selectionRect = btnOptions.bounds;
        else if (selectedItem == 2) selectionRect = btnCredits.bounds;
        else selectionRect = btnExit.bounds;

        DrawRectangleLinesEx({ selectionRect.x - 5, selectionRect.y - 5, selectionRect.width + 10, selectionRect.height + 10 }, 2, Color{96, 96, 0, 255});

        DrawTexture(universityLogoTexture, UI_MARGIN, Config::SCREEN_HEIGHT - universityLogoTexture.height - UI_MARGIN, WHITE);

        const char* yearText = "2026";
        Vector2 yearSize = MeasureTextEx(menuFont, yearText, FOOTER_TEXT_SIZE, 2);
        DrawTextEx(menuFont, yearText, { (float)Config::SCREEN_WIDTH - yearSize.x - 10, (float)Config::SCREEN_HEIGHT - yearSize.y - 10 }, FOOTER_TEXT_SIZE, 2, GRAY);
    }

    ~Title() {
        if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
        if (mainLogoTexture.id > 0) UnloadTexture(mainLogoTexture);
        if (universityLogoTexture.id > 0) UnloadTexture(universityLogoTexture);
        if (menuFont.texture.id > 0) UnloadFont(menuFont);
    }
};

inline void Title::Update(float deltatime, country* country_ptr) {
    introTimer += deltatime;
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
        ExecuteMenuAction(selectedItem, country_ptr);
    } else if (umkClicked) {
        UI::PlaySelectSound();
        OpenURL("https://www.umk.pl/");
    } else {
        for (int i = 0; i < totalItems; i++) {
            if (clicked[i]) ExecuteMenuAction(i, country_ptr);
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) exit(0);
}

inline void GoToTitle(country* country_ptr) {
    StateManager::Instance().ChangeState(new Title(country_ptr));
}

#endif // STATE_TITLE_H
