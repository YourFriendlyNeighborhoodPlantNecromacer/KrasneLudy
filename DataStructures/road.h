#ifndef __ROAD_H__
#define __ROAD_H__
#include <inttypes.h>

class location;

class road{
    private:
    location* destination;
    int64_t maximum_capacity;

    public:
    road();
    road(location* destination_ptr);
    road(location* destination_ptr, int64_t maximum_capacity);

    location* get_destination();
    void dp(int64_t tabulator = 0);
};

#endif//__ROAD_H__

