#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <raylib.h>
#include <src/Constants.h>

namespace UI {
    // Zasoby audio UI
    extern Sound hoverSound;
    extern Sound selectSound;

    // Funkcje do zarządzania dźwiękami UI
    void LoadUISounds();
    void UnloadUISounds();
    void PlayHoverSound();
    void PlaySelectSound();

    inline const char* AssetPath(const char* subPath) {
        return TextFormat("assets/%s", subPath);
    }

    inline Font LoadStandardFont(int fontSize) {
        int codepoints[128] = { 0 };
        for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
        int polish[] = { 0x0104, 0x0105, 0x0106, 0x0107, 0x0118, 0x0119, 0x0141, 0x0142, 0x0143, 0x0144, 0x00D3, 0x00F3, 0x015A, 0x015B, 0x0179, 0x017A, 0x017B, 0x017C };
        for (int i = 0; i < 18; i++) codepoints[95 + i] = polish[i];
        return LoadFontEx(AssetPath("fonts/Ranchers-Regular.ttf"), fontSize, codepoints, 95 + 18);
    }

    inline void DrawTiledBackground(Texture2D texture) {
        for (int x = 0; x < Config::SCREEN_WIDTH; x += texture.width) {
            for (int y = 0; y < Config::SCREEN_HEIGHT; y += texture.height) {
                DrawTexture(texture, x, y, WHITE);
            }
        }
    }

    inline void DrawMenuButton(Rectangle rec, const char* text, Font font) {
        bool hover = CheckCollisionPointRec(GetMousePosition(), rec);
        DrawRectangleRec(rec, hover ? DARKGRAY : GRAY);
        DrawRectangleLinesEx(rec, 2, BLACK);
        Vector2 textSize = MeasureTextEx(font, text, 20, 2);
        DrawTextEx(font, text, { rec.x + (rec.width - textSize.x) / 2, rec.y + 10 }, 20, 2, WHITE);
    }
}

#endif