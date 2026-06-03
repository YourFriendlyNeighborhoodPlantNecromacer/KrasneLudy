#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace Config {
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;

    const std::string ASSET_PATH = "assets";
}

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

#endif // CONSTANTS_H