#ifndef __HOUSE_H__
#define __HOUSE_H__
#include "../src/Constants.h"
#include <inttypes.h>

class house{
    public:
    int64_t index;
    NamedValues::material favorite_material;
    double coordinates[3];

    house(int64_t index);
    house(int64_t index, double x, double y, double z, NamedValues::material favorite_material);
};

#endif // __HOUSE_H__
