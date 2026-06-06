#include <iostream>
#include "../../src/Constants.h"
#include "math.h"
double distance(workplace* a, workplace* b){
    return  sqrt((a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])*(a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])+
            (a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y])*(a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y]));
}

double distance(workplace* a, house* b){
    return  sqrt((a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])*(a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])+
            (a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y])*(a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y]));
}

double distance(house* a, workplace* b){
    return  sqrt((a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])*(a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])+
            (a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y])*(a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y]));
}

double distance(house* a, house* b){
    return  sqrt((a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])*(a->coordinates[namedValues::axis::X] - b->coordinates[namedValues::axis::X])+
            (a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y])*(a->coordinates[namedValues::axis::Y] - b->coordinates[namedValues::axis::Y]));
}


double distance(double* a, double* b){
    return  sqrt((a[namedValues::axis::X] - b[namedValues::axis::X])*(a[namedValues::axis::X] - b[namedValues::axis::X])+
            (a[namedValues::axis::Y] - b[namedValues::axis::Y])*(a[namedValues::axis::Y] - b[namedValues::axis::Y]));
}
