#include <raylib.h>
#include "src/backend/StateManager.h"
#include "src/backend/SettingsManager.h"
#include "src/states/Title.h"
#include "src/Constants.h"
#include "src/backend/UIHelpers.h"
#include "GLUGLU/functions/country_generator.h"
#include <filesystem>

void calculationsAndLogis(country& kingdom, const std::string sourceFile, const std::string outputFileForWorkplaceAssignment, const std::string outputFileForRimPoints){
    kingdom.constructFromFile(sourceFile);
    kingdom.assignDwarfsToWorkplaces(outputFileForWorkplaceAssignment);
    kingdom.saveActiveWorkplaces(kingdom.constructRimAroundCountry(), outputFileForRimPoints);
}

void render_and_graphics(country& kingdom, float screenWidth, float screenHeight, const char* windowName){
    SettingsManager::Instance().LoadSettings();

    InitWindow(screenWidth, screenHeight, windowName);
    SetExitKey(KEY_NULL);
    InitAudioDevice();

    // Zastosuj ustawienia do gry
    SettingsManager::Instance().GetSettings().ApplyToGame();

    // Ładowanie globalnych zasobów audio
    Music globalBGM = LoadMusicStream(UI::AssetPath("music/bgm.mp3"));
    PlayMusicStream(globalBGM);
    UI::LoadUISounds();

    StateManager::Instance().ChangeState(new Title(&kingdom));

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        StateManager::Instance().ProcessStateChange();

        UpdateMusicStream(globalBGM);
        // Dynamiczna aktualizacja głośności muzyki z uwzględnieniem ustawień
        SetMusicVolume(globalBGM, SettingsManager::Instance().GetSettings().musicVolume);

        float deltaTime = GetFrameTime();
        StateManager::Instance().Update(deltaTime, &kingdom);

        BeginDrawing();
            ClearBackground(BLACK);
            StateManager::Instance().Draw();

            if (SettingsManager::Instance().GetSettings().showFPS) {
                DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, LIME);
            }
        EndDrawing();
    }

    StateManager::Instance().Shutdown();
    UI::UnloadUISounds();
    UnloadMusicStream(globalBGM);
    CloseAudioDevice();

    CloseWindow();
}

int main (int argc, char** argv) {
    if(argc > 2){
        std::cerr << "WRONG AMOUNT OF ARGUMENTS, PLEASE INSTERT A SINGLE FILE";
        return -1;
    }
    country kingdom_country;
    std::filesystem::path input_path;
    std::string base_name;
    if(argc == 2){ ///  Został podany plik
        input_path = argv[1];
        base_name = input_path.stem().string();
    }
    if(argc == 1){ /// Nie został podany plik
        base_name = "RandomlyGenerated";
        input_path = base_name + ".txt";
        int64_t precisionPoint = 3;
        int64_t amountOfWorkplacesPerMaterial = 3;
        int64_t amountOfHousesPerMaterial = 3;
        int64_t capacity = 3;
        bool randomized = true;
        int64_t seed = 3;
        generateCountry(input_path, precisionPoint, amountOfWorkplacesPerMaterial, amountOfHousesPerMaterial, capacity, randomized, seed);
    }


    if(std::filesystem::create_directories(base_name)) std::cout << "WOW";
    std::string workplace_file =
    base_name + "/" +
    base_name + "_dwarf_workplace_assignment.txt";

    std::string rim_file =
    base_name + "/" +
    base_name + "_outer_rim.txt";

    calculationsAndLogis(kingdom_country, input_path.string(), workplace_file, rim_file);
    render_and_graphics(kingdom_country, Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "AiSD II - Projekt Krasnoludki");

    /**/
    return 0;
}
