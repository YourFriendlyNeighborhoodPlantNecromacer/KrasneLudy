#ifndef __WORKPLACE_H__
#define __WORKPLACE_H__
#include "../src/Constants.h"
#include <inttypes.h>

class workplace{
    public:
    int64_t index;
    double coordinates[3];

    namedValues::material materialExtracted;
    int64_t capacity;
    int64_t amountOfDwarfsWorking;

    workplace(int64_t index);
    workplace(int64_t index, double x, double y, double z, namedValues::material materialExtracted, int64_t capacity);

    bool isActive();
};

#endif
