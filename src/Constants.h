#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <map>
#include <raylib.h>

namespace NamedValues {
    enum material{
        plants,
        wood,
        clay,
        metal,
        gems,
        garden_gnomes,
        size
    };

    enum errors{
        out_of_range_error = -1,
        not_found_error = -2,
        file_not_found_error = -3
    };

    enum constant{
        tabulator_indent = 4
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

    const std::map<NamedValues::material, Color> MATERIAL_COLORS = {
        {NamedValues::material::plants, Color{40, 212, 104, 255}},
        {NamedValues::material::wood, Color{162, 113, 47, 255}},
        {NamedValues::material::clay, Color{235, 237, 180, 255}},
        {NamedValues::material::metal, Color{152, 190, 208, 255}},
        {NamedValues::material::gems, Color{186, 54, 223, 255}},
        {NamedValues::material::garden_gnomes, Color{241, 86, 86, 255}}
    };
}

#endif // CONSTANTS_H