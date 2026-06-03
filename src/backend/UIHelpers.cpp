#include "UIHelpers.h"

namespace UI {
    Sound hoverSound;
    Sound selectSound;

    void LoadUISounds() {
        hoverSound = LoadSound(AssetPath("sounds/ui/hover.mp3"));
        selectSound = LoadSound(AssetPath("sounds/ui/click.mp3"));
    }

    void UnloadUISounds() {
        UnloadSound(hoverSound);
        UnloadSound(selectSound);
    }

    void PlayHoverSound() {
        PlaySound(hoverSound);
    }
    void PlaySelectSound() {
        PlaySound(selectSound);
    }
}