#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <raylib.h>
#include "../Constants.h"
#include "SettingsManager.h"

namespace UI {
    static constexpr int ASCII_START_CODEPOINT = 32;
    static constexpr int ASCII_GLYPH_COUNT = 95;
    static constexpr int POLISH_GLYPH_COUNT = 18;
    static constexpr int TOTAL_GLYPH_COUNT = ASCII_GLYPH_COUNT + POLISH_GLYPH_COUNT;

    inline Sound& HoverSound() {
        static Sound hoverSound = { 0 };
        return hoverSound;
    }

    inline Sound& SelectSound() {
        static Sound selectSound = { 0 };
        return selectSound;
    }

    inline const char* AssetPath(const char* subPath) {
        return TextFormat("%s/%s", Config::ASSET_PATH.c_str(), subPath);
    }

    inline void LoadUISounds() {
        HoverSound() = LoadSound(AssetPath("sounds/ui/hover.mp3"));
        SelectSound() = LoadSound(AssetPath("sounds/ui/click.mp3"));
    }

    inline void UnloadUISounds() {
        UnloadSound(HoverSound());
        UnloadSound(SelectSound());
    }

    inline void PlayHoverSound() {
        SetSoundVolume(HoverSound(), SettingsManager::Instance().GetSettings().sfxVolume);
        PlaySound(HoverSound());
    }

    inline void PlaySelectSound() {
        SetSoundVolume(SelectSound(), SettingsManager::Instance().GetSettings().sfxVolume);
        PlaySound(SelectSound());
    }

    inline const char* GetBackgroundPath() {
        int idx = SettingsManager::Instance().GetSettings().menuBackgroundIndex;

        return (idx == 0) ? "images/ui/marble.png" :
                (idx == 1 ? "images/ui/wood.png" : "images/ui/water.png");
    }

    // Ładowanie czcionki ze wsparciem dla znaków polskich gdyż to nie standard w raylib
    inline Font LoadStandardFont(int fontSize) {
        int codepoints[TOTAL_GLYPH_COUNT] = { 0 };
        for (int i = 0; i < ASCII_GLYPH_COUNT; i++) codepoints[i] = ASCII_START_CODEPOINT + i;
        int polish[] = { 0x0104, 0x0105, 0x0106, 0x0107, 0x0118, 0x0119, 0x0141, 0x0142, 0x0143, 0x0144, 0x00D3, 0x00F3, 0x015A, 0x015B, 0x0179, 0x017A, 0x017B, 0x017C };
        for (int i = 0; i < POLISH_GLYPH_COUNT; i++) codepoints[ASCII_GLYPH_COUNT + i] = polish[i];
        return LoadFontEx(AssetPath("fonts/Ranchers-Regular.ttf"), fontSize, codepoints, TOTAL_GLYPH_COUNT);
    }

    inline void DrawTiledBackground(Texture2D texture) {
        if (texture.width <= 0 || texture.height <= 0) return;
        for (int x = 0; x < Config::SCREEN_WIDTH; x += texture.width) {
            for (int y = 0; y < Config::SCREEN_HEIGHT; y += texture.height) {
                DrawTexture(texture, x, y, WHITE);
            }
        }
    }
}

#endif
