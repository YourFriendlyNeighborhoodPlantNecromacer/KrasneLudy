#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <map>
#include <raylib.h>

namespace namedValues {
    enum material{
        breadOre,
        wood,
        clay,
        metal,
        gems,
        gardenGnomes,
        size
    };

    enum errors{
        outOfRangeError = -1,
        notFoundError = -2,
        fileNotFoundError = -3
    };

    enum constant{
        tabulatorIndent = 4
    };

    enum axis{
        X,
        Y,
        Z
    };
}

namespace Config {
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;

    const std::string ASSET_PATH = "assets";

    inline float MAP_HALF = 1000.0f;
    inline float MAP_SIZE = 2000.0f;

    const std::map<namedValues::material, Color> MATERIAL_COLORS = {
        {namedValues::material::breadOre, Color{40, 212, 104, 255}},
        {namedValues::material::wood, Color{162, 113, 47, 255}},
        {namedValues::material::clay, Color{235, 237, 180, 255}},
        {namedValues::material::metal, Color{152, 190, 208, 255}},
        {namedValues::material::gems, Color{186, 54, 223, 255}},
        {namedValues::material::gardenGnomes, Color{241, 86, 86, 255}}
    };

    const std::map<namedValues::material, const char*> MATERIAL_NAMES = {
        {namedValues::material::breadOre, "CHLEB"},
        {namedValues::material::wood, "DREWNO"},
        {namedValues::material::clay, "GLINA"},
        {namedValues::material::metal, "METAL"},
        {namedValues::material::gems, "KLEJNOTY"},
        {namedValues::material::gardenGnomes, "KRASNALE OGRODOWE"}
    };

    const Color RIM_COLOR = Color{255,196,0,255};

    constexpr float DECAMETER_DISTANCE = 10.0f;
}

namespace UI {
    constexpr float SELECTOR_PADDING = 5.0f;
    constexpr float SELECTOR_THICKNESS = 2.0f;
    const Color SELECTOR_COLOR = { 96, 96, 0, 255 };

    constexpr float UI_MARGIN = 20.0f;
    constexpr float BACK_BTN_SIZE = 40.0f;

    constexpr float HEADER_FONT_SIZE = 40.0f;
    constexpr float SUBHEADER_FONT_SIZE = 30.0f;
    constexpr float LABEL_FONT_SIZE = 25.0f;
    constexpr float NORMAL_FONT_SIZE = 20.0f;
    constexpr float LIST_ITEM_FONT_SIZE = 18.0f;

    constexpr float BUTTON_WIDTH = 200.0f;
    constexpr float BUTTON_HEIGHT = 40.0f;

    constexpr float FONT_SPACING_HEADER = 2.0f;
    constexpr float FONT_SPACING_NORMAL = 1.0f;
}

#endif // CONSTANTS_H
