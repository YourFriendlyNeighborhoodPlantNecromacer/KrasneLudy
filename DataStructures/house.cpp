#include "house.h"
#include "../src/Constants.h"

house::house(int64_t index, double x, double y, double z, namedValues::material favoriteMaterial) : index(index), favoriteMaterial(favoriteMaterial){
    coordinates[namedValues::axis::X] = x;
    coordinates[namedValues::axis::Y] = y;
    coordinates[namedValues::axis::Z] = z;
}
