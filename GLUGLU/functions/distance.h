#include <iostream>
#include "../../src/Constants.h"
#include "math.h"
double distance(workplace* a, workplace* b){
    return  sqrt((a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])*(a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])+
            (a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y])*(a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y]));
}

double distance(workplace* a, house* b){
    return  sqrt((a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])*(a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])+
            (a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y])*(a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y]));
}

double distance(house* a, workplace* b){
    return  sqrt((a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])*(a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])+
            (a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y])*(a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y]));
}

double distance(house* a, house* b){
    return  sqrt((a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])*(a->coordinates[NamedValues::axis::X] - b->coordinates[NamedValues::axis::X])+
            (a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y])*(a->coordinates[NamedValues::axis::Y] - b->coordinates[NamedValues::axis::Y]));
}


double distance(double* a, double* b){
    return  sqrt((a[NamedValues::axis::X] - b[NamedValues::axis::X])*(a[NamedValues::axis::X] - b[NamedValues::axis::X])+
            (a[NamedValues::axis::Y] - b[NamedValues::axis::Y])*(a[NamedValues::axis::Y] - b[NamedValues::axis::Y]));
}