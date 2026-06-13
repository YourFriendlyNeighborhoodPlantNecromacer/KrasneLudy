#ifndef UI_TEXTFIELD_H
#define UI_TEXTFIELD_H

#include <raylib.h>
#include <string>
#include <algorithm>
#include "../../Constants.h"

namespace UI {
    class TextField {
    public:
        Rectangle bounds;
        std::string text;
        Font font;
        int fontSize;
        float spacing;
        Color textColor;
        Color borderColor;
        Color activeBorderColor;
        Color backgroundColor;
        bool isFocused;
        size_t maxTextLength;
        bool isNumeric; // Pozwala tylko na liczby całkowite

		TextField() = default;
        TextField(Rectangle bounds, Font font, int fontSize, float spacing, Color textColor, Color borderColor, Color activeBorderColor, Color backgroundColor, int maxLen = 20, bool numeric = false)
            : bounds(bounds), text(""), font(font), fontSize(fontSize), spacing(spacing),
              textColor(textColor), borderColor(borderColor), activeBorderColor(activeBorderColor),
              backgroundColor(backgroundColor), isFocused(false), maxTextLength(maxLen), isNumeric(numeric) {}

        void SetText(const std::string& newText) {
            text = newText;
            if (text.length() > maxTextLength) {
                text = text.substr(0, maxTextLength);
            }
        }

        std::string GetText() const {
            return text;
        }

        bool IsFocused() const {
            return isFocused;
        }

        bool Update() {
            bool textChanged = false;
            Vector2 mousePoint = GetMousePosition();

            if (CheckCollisionPointRec(mousePoint, bounds)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    isFocused = true;
                    UI::PlaySelectSound();
                }
            } else {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    isFocused = false;
                }
            }

            if (isFocused) {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();
                while (key > 0) {
                    if ((key >= 32) && (key <= 125) && (text.length() < maxTextLength)) {
                        if (isNumeric) {
                            if (isdigit(key) || (key == '-' && text.empty())) {
                                text += (char)key;
                                textChanged = true;
                            }
                        } else {
                            text += (char)key;
                            textChanged = true;
                        }
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (!text.empty()) {
                        text.pop_back();
                        textChanged = true;
                    }
                }
            }

            return textChanged;
        }

        void Draw() const {
            DrawRectangleRec(bounds, backgroundColor);
            DrawRectangleLinesEx(bounds, 2, isFocused ? activeBorderColor : borderColor);

            Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
            Vector2 textPosition = { bounds.x + 5, bounds.y + (bounds.height - textSize.y) / 2 };
            DrawTextEx(font, text.c_str(), textPosition, fontSize, spacing, textColor);

			// Mrugający kursor
            if (isFocused && ((int)(GetTime() * 2.0f) % 2 == 0)) {
                float cursorX = textPosition.x + textSize.x;
                DrawLineV({cursorX, bounds.y + (bounds.height - textSize.y) / 2},
                          {cursorX, bounds.y + (bounds.height + textSize.y) / 2}, textColor);
            }
        }
    };
}

#endif // UI_TEXTFIELD_H