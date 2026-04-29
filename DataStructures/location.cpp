#include "location.h"
#include <iostream>
#include <inttypes.h>

location::location(int64_t index) : index(index), connected_to(), type(building_type::debug){
    for(int i=0;i<3;i++)coordinates[i]=0;
}

location::location(int64_t index, double X, double Y, double Z) : index(index), connected_to(), type(building_type::debug){
    coordinates[axis::X]=X;
    coordinates[axis::Y]=Y;
    coordinates[axis::Z]=Z;
}

bool location::is_connected_to(int64_t index){
    for(int i=0; i<connected_to.get_last_index(); i++){
        if(index==connected_to[i]->get_destination()->index) return true;
    }
    return false;
}

void location::add_road_to(location* road_destination, int64_t maximum_capacity){
    connected_to.append(new road(road_destination, maximum_capacity));
}

int64_t location::get_amount_of_roads(){
    return connected_to.get_last_index();
}

void location::randomise_location(){
    for(int i=0; i<3; i++){
        coordinates[i] = std::rand()%100/100.0;
    }
}

void location::set_location(double new_x, double new_y, double new_z){
    coordinates[axis::X] = new_x;
    coordinates[axis::Y] = new_y;
    coordinates[axis::Z] = new_z;
}


void location::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << "COORDINATES: " << std::endl;
    tab(tabulation);
    std::cout << "X: " << coordinates[axis::X] << "   Y: " << coordinates[axis::Y] << "   Z: " << coordinates[axis::Z] << std::endl;
    connected_to.dp(tabulation+3);
}

std::ostream& operator << (std::ostream &os, location data){
    for(int i=0; i < data.get_amount_of_roads(); i++) os << i;
    return os;
}
