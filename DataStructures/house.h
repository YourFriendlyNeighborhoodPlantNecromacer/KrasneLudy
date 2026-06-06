#ifndef __HOUSE_H__
#define __HOUSE_H__
#include "../src/Constants.h"
#include <inttypes.h>

class house{
    public:
    int64_t index;
    namedValues::material favoriteMaterial;
    double coordinates[3];

    house(int64_t index);
    house(int64_t index, double x, double y, double z, namedValues::material favoriteMaterial);
};

#endif // __HOUSE_H__
