#include "workplace.h"
#include <inttypes.h>
#include "../src/Constants.h"

workplace::workplace(int64_t index, double x, double y, double z, NamedValues::material material_extracted, int64_t capacity): index(index), material_extracted(material_extracted), capacity(capacity), amount_of_dwarfs_working(0){
    coordinates[NamedValues::axis::X] = x;
    coordinates[NamedValues::axis::Y] = y;
    coordinates[NamedValues::axis::Z] = z;
}

bool workplace::is_active(){return amount_of_dwarfs_working > 0;}
