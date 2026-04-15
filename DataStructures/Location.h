#ifndef __LOCATIONS_H__
#define __LOCATIONS_H__

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

    struct road{
        //properties...;
        location* leads_to;
        road():leads_to(nullptr){};
    };

    dynamic_array<road> connected_to;
    building_type type;
    double coordinates[3]; ///X Y ELEVATION
    //autosorted_dynamic_array<location> a;

    void dp(){
        std::cout << "COORDINATES: " << std::endl;
        std::cout << "X:" << coordinates[axis::X] << "   Y:" << coordinates[axis::Y] << "   Z:" << coordinates[axis::Z] << std::endl;
    }
};

std::ostream& operator << (std::ostream &os, const location structure){
    os << "Can i get number fifteennn";
    return os;
}

#endif//__LOCATIONS_H__


// 29 kwietnia (45)
