#ifndef __LOCATIONS_H__
#define __LOCATIONS_H__

enum location_type{
    debug = -1,
    road = 0,
    house = 1,
    workplace = 2
};

class location{
    public:
    enum axis{
        x,
        y,
        elevation
    };

    location();
    location(location_type type) : type(type), coordinates(){};
    location(double* coordinates) : type(location_type::debug), coordinates(){}
    location(location_type type, double* coordinates) : type(type), coordinates(){
        for(int i=0; i<3; i++)this->coordinates[0] = coordinates[0];
    };

    // copyconst

    location_type type;
    double coordinates[3]; ///X Y ELEVATION
    //autosorted_dynamic_array<location> a;
};



#endif//__LOCATIONS_H__
