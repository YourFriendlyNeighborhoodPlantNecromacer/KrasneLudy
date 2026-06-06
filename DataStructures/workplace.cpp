#include "workplace.h"
#include <inttypes.h>
#include "../src/Constants.h"

workplace::workplace(int64_t index, double x, double y, double z, namedValues::material materialExtracted, int64_t capacity): index(index), materialExtracted(materialExtracted), capacity(capacity), amountOfDwarfsWorking(0){
    coordinates[namedValues::axis::X] = x;
    coordinates[namedValues::axis::Y] = y;
    coordinates[namedValues::axis::Z] = z;
}

bool workplace::isActive(){return amountOfDwarfsWorking > 0;}
