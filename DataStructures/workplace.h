#ifndef __WORKPLACE_H__
#define __WORKPLACE_H__
#include "../src/Constants.h"
#include <inttypes.h>

class workplace{
    public:
    int64_t index;
    double coordinates[3];

    NamedValues::material material_extracted;
    int64_t capacity;
    int64_t amount_of_dwarfs_working;

    workplace(int64_t index);
    workplace(int64_t index, double x, double y, double z, NamedValues::material material_extracted, int64_t capacity);

    bool is_active();
};

#endif
