#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <raylib.h>
#include <algorithm>

namespace UI {
    class Button {
    public:
        static constexpr float DEFAULT_BORDER_THICKNESS = 2.0f;

        Rectangle bounds = { 0 };
        const char* text = "";
        Font font = { 0 };
        Color regularColor = GRAY;
        Color hoverColor = DARKGRAY;
        Color textColor = WHITE;
        Color borderColor = BLACK;
        int fontSize = 20;
        float spacing = 2.0f;
        bool isHovered = false;

        Button() = default;
        Button(Rectangle bounds, const char* text, Font font, Color regularColor = GRAY, Color hoverColor = DARKGRAY, Color textColor = WHITE, Color borderColor = BLACK, int fontSize = 20, float spacing = 2.0f);

        void SetBounds(Rectangle rect);
        void SetText(const char* newText);
        void SetFont(Font newFont);
        bool Update();
        void Draw() const;
        bool IsHovered() const;
        bool IsPressed() const;
    };

    inline Button::Button(Rectangle bounds, const char* text, Font font, Color regularColor, Color hoverColor, Color textColor, Color borderColor, int fontSize, float spacing)
        : bounds(bounds)
        , text(text)
        , font(font)
        , regularColor(regularColor)
        , hoverColor(hoverColor)
        , textColor(textColor)
        , borderColor(borderColor)
        , fontSize(fontSize)
        , spacing(spacing)
        , isHovered(false)
    {}

    inline void Button::SetBounds(Rectangle rect) {
        bounds = rect;
    }

    inline void Button::SetText(const char* newText) {
        text = newText;
    }

    inline void Button::SetFont(Font newFont) {
        font = newFont;
    }

    inline bool Button::Update() {
        isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
        return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    inline bool Button::IsHovered() const {
        return CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    inline bool Button::IsPressed() const {
        return IsHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    inline void Button::Draw() const {
        DrawRectangleRec(bounds, isHovered ? hoverColor : regularColor);
        DrawRectangleLinesEx(bounds, DEFAULT_BORDER_THICKNESS, borderColor);

        if (font.texture.id != 0 && text != nullptr) {
            Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
            Vector2 position = { bounds.x + (bounds.width - textSize.x) / 2.0f, bounds.y + (bounds.height - textSize.y) / 2.0f };
            DrawTextEx(font, text, position, fontSize, spacing, textColor);
        }
    }
}

#endif // UI_BUTTON_H
