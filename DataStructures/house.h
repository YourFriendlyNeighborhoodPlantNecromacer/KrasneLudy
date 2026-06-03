#ifndef __HOUSE_H__
#define __HOUSE_H__
#include "../constants.h"
#include <inttypes.h>

class house{
    public:
    int64_t index;
    material favorite_material;
    double coordinates[3];

    house(int64_t index);
    house(int64_t index, double x, double y, double z, material favorite_material);
};

#endif // __HOUSE_H__
