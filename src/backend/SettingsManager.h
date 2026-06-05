#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <raylib.h>

struct GameSettings {
    // Ustawienia Wyświetlania
    bool fullscreen = false;
    bool vsync = true;
    int targetFPS = 60;

    // Ustawienia Audio
    float masterVolume = 1.0f;
    float musicVolume = 1.0f;
    float sfxVolume = 1.0f;

    // Inne Ustawienia
    bool showFPS = false;
    int menuBackgroundIndex = 0;

    void ApplyToGame() const {
        if (vsync) SetWindowState(FLAG_VSYNC_HINT);
        else ClearWindowState(FLAG_VSYNC_HINT);

        SetMasterVolume(masterVolume);

        SetTargetFPS(targetFPS);

        if (fullscreen != IsWindowFullscreen()) {
            ToggleFullscreen();
        }
    }
};

class SettingsManager {
private:
    static constexpr const char* SETTINGS_FILE = "settings.cfg";
    GameSettings settings;

    // Pomocnik do usuwania białych znaków z ciągów znaków
    static std::string Trim(std::string s) {
        size_t first = s.find_first_not_of(" \t");
        if (first == std::string::npos) return "";
        size_t last = s.find_last_not_of(" \t");
        return s.substr(first, (last - first + 1));
    }

    // Przeciążone pomocniki do skróconego parsowania
    void Parse(bool& m, const std::string& v) { m = (v == "true" || v == "1"); }
    void Parse(int& m, const std::string& v)  { m = std::stoi(v); }
    void Parse(float& m, const std::string& v){ m = std::stof(v); }

    SettingsManager() {}

public:
    static SettingsManager& Instance() {
        static SettingsManager instance;
        return instance;
    }

    const GameSettings& GetSettings() const {
        return settings;
    }

    GameSettings& GetSettingsMutable() {
        return settings;
    }

    void LoadSettings() {
        std::ifstream file(SETTINGS_FILE);
        if (!file.is_open()) { // Użyj domyślnych ustawień jeżeli jest błąd
            SaveSettings();
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; // Komentarz

            size_t delimPosition = line.find('=');
            if (delimPosition == std::string::npos) continue; // Wartość

            std::string key = Trim(line.substr(0, delimPosition));
            std::string value = Trim(line.substr(delimPosition + 1));

            // Przejdź przez ustawienia
            if (key == "fullscreen")           Parse(settings.fullscreen, value);
            else if (key == "vsync")           Parse(settings.vsync, value);
            else if (key == "masterVolume")    Parse(settings.masterVolume, value);
            else if (key == "musicVolume")     Parse(settings.musicVolume, value);
            else if (key == "sfxVolume")       Parse(settings.sfxVolume, value);
            else if (key == "showFPS")         Parse(settings.showFPS, value);
            else if (key == "menuBackgroundIndex") Parse(settings.menuBackgroundIndex, value);
            else if (key == "targetFPS")       Parse(settings.targetFPS, value);
        }

        file.close();
    }

    void SaveSettings() const {
        std::ofstream file(SETTINGS_FILE);
        if (!file.is_open()) return;

        file << "# Projekt Krasnoludki: Opcje\n";
        file << "# Wyświetlanie\n";
        file << "fullscreen=" << (settings.fullscreen ? "true" : "false") << "\n";
        file << "vsync=" << (settings.vsync ? "true" : "false") << "\n";
        file << "targetFPS=" << settings.targetFPS << "\n";
        file << "\n# Dźwięk\n";
        file << "masterVolume=" << settings.masterVolume << "\n";
        file << "musicVolume=" << settings.musicVolume << "\n";
        file << "sfxVolume=" << settings.sfxVolume << "\n";
        file << "\n# Inne\n";
        file << "showFPS=" << (settings.showFPS ? "true" : "false") << "\n";
        file << "menuBackgroundIndex=" << settings.menuBackgroundIndex << "\n";

        file.close();
    }

    void ResetToDefaults() {
        settings = GameSettings();
        SaveSettings();
    }
};

#endif // SETTINGS_MANAGER_H
