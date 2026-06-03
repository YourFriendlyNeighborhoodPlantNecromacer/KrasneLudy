#include "workplace.h"
#include <inttypes.h>
#include "../constants.h"

workplace::workplace(int64_t index, double x, double y, double z, material material_extracted, int64_t capacity): index(index), material_extracted(material_extracted), capacity(capacity), amount_of_dwarfs_working(0){
    coordinates[axis::X] = x;
    coordinates[axis::Y] = y;
    coordinates[axis::Z] = z;
}

bool workplace::is_active(){return amount_of_dwarfs_working > 0;}
