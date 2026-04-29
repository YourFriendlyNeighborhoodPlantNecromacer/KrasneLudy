#include <iostream>
#include "road.h"
#include <inttypes.h>
#include "../GLUGLU/functions/tabulator.h"

road::road():destination(nullptr){}
road::road(location* destination_ptr) : destination(destination_ptr){}
road::road(location* destination_ptr, int64_t maximum_capacity) : destination(destination_ptr), maximum_capacity(maximum_capacity){}

std::ostream& operator << (std::ostream& os, road data){ /// const to road would help microoptimize
    if(data.get_destination()){
        os << "Road leads to: " << data.get_destination() << std::endl;
    }
    else
    {
        os << "Road leads to: Nowhere!" << std::endl;
    }
    return os;
}

location* road::get_destination(){
    return destination;
};

void road::dp(int64_t tabulator){
    tab(tabulator);
    std::cout << "Destination: " << destination << "     Maximum capacity: " << maximum_capacity << std::endl;
}
