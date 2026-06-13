#ifndef STATE_GENERATION_H
#define STATE_GENERATION_H

#include <string>
#include <filesystem>
#include <vector>
#include "../backend/GameState.h"
#include "../backend/StateManager.h"
#include "../backend/UIHelpers.h"
#include "../backend/classes/TextField.h"
#include "../backend/classes/Button.h"
#include "../backend/GraphProcessing.h"
#include "../Constants.h"
#include "../../GLUGLU/functions/country_generator.h"
#include "Title.h"
#include <thread>

class Generation : public GameState {
public:
    // UI Layout Constants
    static constexpr float UI_FIELD_WIDTH = 200.0f;
    static constexpr float UI_FIELD_HEIGHT = 30.0f;
    static constexpr float UI_FIELD_SPACING = 60.0f;
    static constexpr float UI_START_Y = 140.0f;
    static constexpr float UI_LABEL_OFFSET = 240.0f;
    static constexpr float UI_BUTTON_WIDTH = 200.0f;
    static constexpr float UI_BUTTON_HEIGHT = 50.0f;
    static constexpr float UI_BUTTON_GAP = 20.0f;
    static constexpr float UI_BOTTOM_MARGIN = 80.0f;
    static constexpr float UI_HEADER_Y = 40.0f;
    static constexpr float UI_ERROR_MARGIN_Y = 120.0f;

    // Field Constraints
    static constexpr int MAX_LEN_FILENAME = 20;
    static constexpr int MAX_LEN_SMALL_VAL = 5;
    static constexpr int MAX_LEN_SEED = 10;

    // Randomization Limits
    static constexpr int RAND_PRECISION_MIN = 1;
    static constexpr int RAND_PRECISION_MAX = 7;
    static constexpr int RAND_VAL_MIN = 5;
    static constexpr int RAND_VAL_MAX = 1000;
    static constexpr int RAND_SEED_MAX = 9999999;

    country* countryPtr;
    Font font;
    Font headerFont;
    Texture2D backgroundTexture;

    UI::TextField filenameField;
    UI::TextField precisionField;
    UI::TextField workplacesField;
    UI::TextField housesField;
    UI::TextField capacityField;
    UI::TextField seedField;

    UI::Button btnRandomize;
    UI::Button btnGenerate;

    std::string filenameStr = "new_kingdom";
    long long precisionVal = 3;
    long long workplacesVal = 5;
    long long housesVal = 5;
    long long capacityVal = 5;
    bool randomizedVal = true;
    long long seedVal = 7;

    static constexpr const char* LOADING_MESSAGE = "TRWA GENEROWANIE KRÓLESTWA...";

    std::string errorMessage = "";
	bool isProcessing = false;
    std::thread processingThread;

    Generation(country* ptr) : countryPtr(ptr), font{0}, headerFont{0}, backgroundTexture{0}
    {}

    void Init() override {
        font = UI::LoadStandardFont(UI::NORMAL_FONT_SIZE);
        headerFont = UI::LoadStandardFont(UI::HEADER_FONT_SIZE);
        backgroundTexture = LoadTexture(UI::AssetPath(UI::GetBackgroundPath()));

        float centerX = Config::SCREEN_WIDTH / 2.0f;

        filenameField = UI::TextField({ centerX - (UI_FIELD_WIDTH / 2), UI_START_Y, UI_FIELD_WIDTH, UI_FIELD_HEIGHT }, font, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK, BLACK, UI::SELECTOR_COLOR, RAYWHITE, MAX_LEN_FILENAME);
        filenameField.SetText(filenameStr);

        precisionField = UI::TextField({ centerX - (UI_FIELD_WIDTH / 2), UI_START_Y + UI_FIELD_SPACING, UI_FIELD_WIDTH, UI_FIELD_HEIGHT }, font, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK, BLACK, UI::SELECTOR_COLOR, RAYWHITE, MAX_LEN_SMALL_VAL, true);
        precisionField.SetText(std::to_string(precisionVal));

        workplacesField = UI::TextField({ centerX - (UI_FIELD_WIDTH / 2), UI_START_Y + 2 * UI_FIELD_SPACING, UI_FIELD_WIDTH, UI_FIELD_HEIGHT }, font, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK, BLACK, UI::SELECTOR_COLOR, RAYWHITE, MAX_LEN_SMALL_VAL, true);
        workplacesField.SetText(std::to_string(workplacesVal));

        housesField = UI::TextField({ centerX - (UI_FIELD_WIDTH / 2), UI_START_Y + 3 * UI_FIELD_SPACING, UI_FIELD_WIDTH, UI_FIELD_HEIGHT }, font, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK, BLACK, UI::SELECTOR_COLOR, RAYWHITE, MAX_LEN_SMALL_VAL, true);
        housesField.SetText(std::to_string(housesVal));

        capacityField = UI::TextField({ centerX - (UI_FIELD_WIDTH / 2), UI_START_Y + 4 * UI_FIELD_SPACING, UI_FIELD_WIDTH, UI_FIELD_HEIGHT }, font, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK, BLACK, UI::SELECTOR_COLOR, RAYWHITE, MAX_LEN_SMALL_VAL, true);
        capacityField.SetText(std::to_string(capacityVal));

        seedField = UI::TextField({ centerX - (UI_FIELD_WIDTH / 2), UI_START_Y + 5 * UI_FIELD_SPACING, UI_FIELD_WIDTH, UI_FIELD_HEIGHT }, font, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK, BLACK, UI::SELECTOR_COLOR, RAYWHITE, MAX_LEN_SEED, true);
        seedField.SetText(std::to_string(seedVal));

        float buttonsY = (float)Config::SCREEN_HEIGHT - UI_BOTTOM_MARGIN;
        btnRandomize = UI::Button({ centerX - UI_BUTTON_WIDTH - (UI_BUTTON_GAP / 2.0f), buttonsY, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT }, "LOSUJ", headerFont);
        btnGenerate = UI::Button({ centerX + (UI_BUTTON_GAP / 2.0f), buttonsY, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT }, "GENERUJ", headerFont);
    }

    void Update(float deltaTime, country* ptr) override {
		if (isProcessing) return; //Blokada aktualizacji w trakcie przetwarzania

        filenameField.Update();
        precisionField.Update();
        workplacesField.Update();
        housesField.Update();
        capacityField.Update();
        seedField.Update();

        if (btnRandomize.Update()) {
            UI::PlaySelectSound();

            precisionVal = GetRandomValue(RAND_PRECISION_MIN, RAND_PRECISION_MAX);
            workplacesVal = GetRandomValue(RAND_VAL_MIN, RAND_VAL_MAX);
            housesVal = GetRandomValue(RAND_VAL_MIN, RAND_VAL_MAX);
            capacityVal = GetRandomValue(RAND_VAL_MIN, RAND_VAL_MAX);
            seedVal = GetRandomValue(RAND_PRECISION_MIN, RAND_SEED_MAX);

            precisionField.SetText(std::to_string(precisionVal));
            workplacesField.SetText(std::to_string(workplacesVal));
            housesField.SetText(std::to_string(housesVal));
            capacityField.SetText(std::to_string(capacityVal));
            seedField.SetText(std::to_string(seedVal));
        }

        if (btnGenerate.Update()) {
            std::string currentErrorMessage = "";

            filenameStr = filenameField.GetText();
            if (filenameStr.empty()) {
                currentErrorMessage += "Nazwa pliku nie może być pusta!\n";
            }

            try {
                precisionVal = std::stoll(precisionField.GetText());
                if (precisionVal <= 0) currentErrorMessage += "Precyzja musi być większa od 0!\n";
            } catch (const std::invalid_argument& e) { currentErrorMessage += "Nieprawidłowa precyzja!\n"; }
            catch (const std::out_of_range& e) { currentErrorMessage += "Precyzja poza zakresem!\n"; }

            try {
                workplacesVal = std::stoll(workplacesField.GetText());
                if (workplacesVal <= 0) currentErrorMessage += "Miejsca pracy muszą być większe od 0!\n";
            } catch (const std::invalid_argument& e) { currentErrorMessage += "Nieprawidłowa ilość miejsc pracy!\n"; }
            catch (const std::out_of_range& e) { currentErrorMessage += "Ilość miejsc pracy poza zakresem!\n"; }

            try {
                housesVal = std::stoll(housesField.GetText());
                if (housesVal <= 0) currentErrorMessage += "Domy muszą być większe od 0!\n";
            } catch (const std::invalid_argument& e) { currentErrorMessage += "Nieprawidłowa ilość domów!\n"; }
            catch (const std::out_of_range& e) { currentErrorMessage += "Ilość domów poza zakresem!\n"; }

            try {
                capacityVal = std::stoll(capacityField.GetText());
                if (capacityVal <= 0) currentErrorMessage += "Pojemność musi być większa od 0!\n";
            } catch (const std::invalid_argument& e) { currentErrorMessage += "Nieprawidłowa pojemność!\n"; }
            catch (const std::out_of_range& e) { currentErrorMessage += "Pojemność poza zakresem!\n"; }

            try {
                seedVal = std::stoll(seedField.GetText());
            } catch (const std::invalid_argument& e) { currentErrorMessage += "Nieprawidłowy seed!\n"; }
            catch (const std::out_of_range& e) { currentErrorMessage += "Seed poza zakresem!\n"; }

            errorMessage = currentErrorMessage;

            if (errorMessage.empty()) {
                UI::PlaySelectSound();
                isProcessing = true;

                // Uruchomienie ciężkich obliczeń w osobnym wątku
                processingThread = std::thread([this]() {
                    generateCountry(filenameStr + ".txt", precisionVal, workplacesVal, housesVal, capacityVal, randomizedVal, seedVal);
                    std::filesystem::create_directories(filenameStr);
                    ProcessCountryData(*countryPtr, filenameStr);

                    // Powrót do wątku głównego (przez flagę)
                    isProcessing = false;
                });
            }
        }

        // Cursor feedback
        bool anyHovered = btnGenerate.IsHovered() || btnRandomize.IsHovered() ||
                          filenameField.IsFocused() || precisionField.IsFocused() || workplacesField.IsFocused() ||
                          housesField.IsFocused() || capacityField.IsFocused() || seedField.IsFocused();

        if (anyHovered) SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

	void LateUpdate() override {
        if (!isProcessing && processingThread.joinable()) {
            processingThread.join();
            StateManager::Instance().ChangeState(new Title(countryPtr));
        }
    }

    void Draw() override {
        UI::DrawTiledBackground(backgroundTexture);
        float centerX = Config::SCREEN_WIDTH / 2.0f;

        const char* titleText = "PARAMETRY KRAINY";
        Vector2 titleSize = MeasureTextEx(headerFont, titleText, UI::HEADER_FONT_SIZE, UI::FONT_SPACING_HEADER);
        DrawTextEx(headerFont, titleText, { (Config::SCREEN_WIDTH - titleSize.x) / 2, UI_HEADER_Y }, UI::HEADER_FONT_SIZE, UI::FONT_SPACING_HEADER, BLACK);

        DrawTextEx(font, "Nazwa pliku:", { centerX - UI_LABEL_OFFSET, UI_START_Y + (filenameField.bounds.height - MeasureTextEx(font, "Nazwa pliku:", UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL).y) / 2 }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK);
        filenameField.Draw();

        DrawTextEx(font, "Precyzja:", { centerX - UI_LABEL_OFFSET, UI_START_Y + UI_FIELD_SPACING + (precisionField.bounds.height - MeasureTextEx(font, "Precyzja:", UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL).y) / 2 }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK);
        precisionField.Draw();

        DrawTextEx(font, "Miejsca pracy:", { centerX - UI_LABEL_OFFSET, UI_START_Y + 2 * UI_FIELD_SPACING + (workplacesField.bounds.height - MeasureTextEx(font, "Miejsca pracy:", UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL).y) / 2 }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK);
        workplacesField.Draw();

        DrawTextEx(font, "Domy:", { centerX - UI_LABEL_OFFSET, UI_START_Y + 3 * UI_FIELD_SPACING + (housesField.bounds.height - MeasureTextEx(font, "Domy:", UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL).y) / 2 }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK);
        housesField.Draw();

        DrawTextEx(font, "Pojemność:", { centerX - UI_LABEL_OFFSET, UI_START_Y + 4 * UI_FIELD_SPACING + (capacityField.bounds.height - MeasureTextEx(font, "Pojemność:", UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL).y) / 2 }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK);
        capacityField.Draw();

        DrawTextEx(font, "Seed:", { centerX - UI_LABEL_OFFSET, UI_START_Y + 5 * UI_FIELD_SPACING + (seedField.bounds.height - MeasureTextEx(font, "Seed:", UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL).y) / 2 }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, BLACK);
        seedField.Draw();

        btnRandomize.Draw();
        btnGenerate.Draw();

		if (isProcessing) {
            // Prosty overlay ładowania
            DrawRectangle(0, 0, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, Fade(BLACK, 0.5f));
            Vector2 size = MeasureTextEx(headerFont, LOADING_MESSAGE, UI::LABEL_FONT_SIZE, UI::FONT_SPACING_NORMAL);
            DrawTextEx(headerFont, LOADING_MESSAGE, { centerX - size.x / 2, Config::SCREEN_HEIGHT / 2.0f }, UI::LABEL_FONT_SIZE, UI::FONT_SPACING_NORMAL, RAYWHITE);
        }

        if (!errorMessage.empty()) {
            Vector2 errorSize = MeasureTextEx(font, errorMessage.c_str(), UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL);
            DrawTextEx(font, errorMessage.c_str(), { (Config::SCREEN_WIDTH - errorSize.x) / 2, Config::SCREEN_HEIGHT - UI_ERROR_MARGIN_Y }, UI::NORMAL_FONT_SIZE, UI::FONT_SPACING_NORMAL, RED);
        }
    }

    ~Generation() {
        if (backgroundTexture.id > 0) UnloadTexture(backgroundTexture);
		if (processingThread.joinable()) processingThread.join();
        if (font.texture.id > 0) UnloadFont(font);
    }
};

#endif