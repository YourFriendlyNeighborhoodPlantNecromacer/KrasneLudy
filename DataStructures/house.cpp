#include "house.h"
#include "../src/Constants.h"

house::house(int64_t index, double x, double y, double z, NamedValues::material favorite_material) : index(index), favorite_material(favorite_material){
    coordinates[NamedValues::axis::X] = x;
    coordinates[NamedValues::axis::Y] = y;
    coordinates[NamedValues::axis::Z] = z;
}
