#ifndef UI_SLIDER_H
#define UI_SLIDER_H

#include <raylib.h>
#include <cmath>
#include <cstdio>

namespace UI {
    class Slider {
    public:
        static constexpr float DEFAULT_KNOB_WIDTH = 20.0f;
        static constexpr float DEFAULT_KNOB_HEIGHT = 30.0f;
        static constexpr float BORDER_THICKNESS = 1.0f;
        static constexpr float LABEL_VERTICAL_PADDING = 8.0f;

        Rectangle track = { 0 };
        float minValue = 0.0f;
        float maxValue = 1.0f;
        float value = 0.0f;
        float step = 0.0f;
        float knobWidth = 20.0f;
        float knobHeight = 30.0f;
        bool dragging = false;
        bool hover = false;
        Color trackColor = DARKGRAY;
        Color knobColor = RAYWHITE;
        Color knobHoverColor = LIGHTGRAY;
        Color borderColor = BLACK;
        Font font = { 0 };
        const char* label = "";
        int fontSize = 20;
        float textSpacing = 2.0f;

        Slider() = default;
        Slider(Rectangle track, float minValue, float maxValue, float initialValue, float step, const char* label, Font font, int fontSize = 20, float knobWidth = 20.0f, float knobHeight = 30.0f, Color trackColor = DARKGRAY, Color knobColor = RAYWHITE, Color knobHoverColor = LIGHTGRAY, Color borderColor = BLACK, float textSpacing = 2.0f);

        void SetBounds(Rectangle rect);
        void SetValue(float newValue);
        float GetValue() const;
        float GetProgress() const;
        bool Update();
        void Draw() const;
        bool IsHovered() const;
        Rectangle GetKnobBounds() const;
    };

    inline Slider::Slider(Rectangle track, float minValue, float maxValue, float initialValue, float step, const char* label, Font font, int fontSize, float knobWidth, float knobHeight, Color trackColor, Color knobColor, Color knobHoverColor, Color borderColor, float textSpacing)
        : track(track)
        , minValue(minValue)
        , maxValue(maxValue)
        , value(0.0f)
        , step(step)
        , knobWidth(knobWidth > 0 ? knobWidth : DEFAULT_KNOB_WIDTH)
        , knobHeight(knobHeight > 0 ? knobHeight : DEFAULT_KNOB_HEIGHT)
        , trackColor(trackColor)
        , knobColor(knobColor)
        , knobHoverColor(knobHoverColor)
        , borderColor(borderColor)
        , font(font)
        , label(label)
        , fontSize(fontSize)
        , textSpacing(textSpacing)
    {
        SetValue(initialValue);
    }

    inline void Slider::SetBounds(Rectangle rect) {
        track = rect;
    }

    inline void Slider::SetValue(float newValue) {
        float val = std::max(minValue, std::min(newValue, maxValue));
        if (step > 0.0f) {
            float stepped = std::round((val - minValue) / step) * step + minValue;
            val = std::max(minValue, std::min(stepped, maxValue));
        }
        value = val;
    }

    inline float Slider::GetValue() const {
        return value;
    }

    inline float Slider::GetProgress() const {
        if (maxValue == minValue) return 0.0f;
        return (value - minValue) / (maxValue - minValue);
    }

    inline Rectangle Slider::GetKnobBounds() const {
        float progress = GetProgress();
        float x = track.x + progress * track.width - knobWidth / 2.0f;
        float y = track.y - (knobHeight - track.height) / 2.0f;
        return { x, y, knobWidth, knobHeight };
    }

    inline bool Slider::Update() {
        Vector2 mouse = GetMousePosition();
        Rectangle knob = GetKnobBounds();
        hover = CheckCollisionPointRec(mouse, track) || CheckCollisionPointRec(mouse, knob);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hover) {
            dragging = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            dragging = false;
        }

        if (dragging) {
            float mouseX = std::max(track.x, std::min(mouse.x, track.x + track.width));
            float newValue = minValue + (mouseX - track.x) / track.width * (maxValue - minValue);

            if (step > 0.0f) {
                newValue = std::round((newValue - minValue) / step) * step + minValue;
                newValue = std::max(minValue, std::min(newValue, maxValue));
            }

            if (newValue != value) {
                value = newValue;
                return true;
            }
        }

        return false;
    }

    inline void Slider::Draw() const {
        DrawRectangleRec(track, trackColor);
        DrawRectangleLinesEx(track, BORDER_THICKNESS, borderColor);

        Rectangle knob = GetKnobBounds();
        bool knobHover = hover || dragging;
        DrawRectangleRec(knob, knobHover ? knobHoverColor : knobColor);
        DrawRectangleLinesEx(knob, BORDER_THICKNESS, borderColor);

        if (font.texture.id != 0 && label != nullptr) {
            Vector2 labelSize = MeasureTextEx(font, label, fontSize, textSpacing);
            DrawTextEx(font, label, { track.x, track.y - labelSize.y - LABEL_VERTICAL_PADDING }, fontSize, textSpacing, WHITE);
            char valueText[32];
            int len = snprintf(valueText, sizeof(valueText), "%.0f", value);
            if (len > 0) {
                Vector2 valueSize = MeasureTextEx(font, valueText, fontSize, textSpacing);
                DrawTextEx(font, valueText, { track.x + track.width - valueSize.x, track.y - valueSize.y - 8 }, fontSize, textSpacing, WHITE);
            }
        }
    }

    inline bool Slider::IsHovered() const {
        Vector2 mouse = GetMousePosition();
        return CheckCollisionPointRec(mouse, track) || CheckCollisionPointRec(mouse, GetKnobBounds());
    }
}

#endif // UI_SLIDER_H
