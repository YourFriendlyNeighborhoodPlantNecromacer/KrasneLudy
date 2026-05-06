#ifndef __LOCATIONS_H__
#define __LOCATIONS_H__

#include "../GLUGLU/datastructures/linear/dynamic_array/dynamic_array.h"
#include "road.h"

class location{
    public:
    enum axis{
        X,
        Y,
        Z
    };
    enum building_type{
        debug = -1,
        path = 0,
        house = 1,
        workplace = 2
    };
    int64_t index;
    dynamic_array<road*> connected_to;
    building_type type;
    double coordinates[3]; ///X Y Z

    location(int64_t index);
    location(int64_t index, double X, double Y, double Z);
    ~location();

    void add_road_to(location* road_destination, int64_t maximum_capacity);
    int64_t get_amount_of_roads();
    bool is_connected_to(int64_t index);
    void randomise_location();
    void set_location(double new_x, double new_y, double new_z = 0);

    void dp(int64_t tabulation = 0);
};

double distance(location* p1, location* p2);

#endif//__LOCATIONS_H__

// Algorithms by Dasgupta et al would be easier to read
// Algorithm Design by Kleinberg and Tardos is another popular favorite |||
// A* search
// 29 kwietnia (45)
