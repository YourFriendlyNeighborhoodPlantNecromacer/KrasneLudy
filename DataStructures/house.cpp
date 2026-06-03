#include "house.h"

house::house(int64_t index, double x, double y, double z, material favorite_material) : index(index), favorite_material(favorite_material){
    coordinates[axis::X] = x;
    coordinates[axis::Y] = y;
    coordinates[axis::Z] = z;
}
