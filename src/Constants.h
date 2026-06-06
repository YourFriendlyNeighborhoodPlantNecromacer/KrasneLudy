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
}

#endif // CONSTANTS_H
