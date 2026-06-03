#include <iostream>
#include "../../constants.h"
#include "math.h"
double distance(workplace* a, workplace* b){
    return  sqrt((a->coordinates[axis::X] - b->coordinates[axis::X])*(a->coordinates[axis::X] - b->coordinates[axis::X])+
            (a->coordinates[axis::Y] - b->coordinates[axis::Y])*(a->coordinates[axis::Y] - b->coordinates[axis::Y]));
}

double distance(workplace* a, house* b){
    return  sqrt((a->coordinates[axis::X] - b->coordinates[axis::X])*(a->coordinates[axis::X] - b->coordinates[axis::X])+
            (a->coordinates[axis::Y] - b->coordinates[axis::Y])*(a->coordinates[axis::Y] - b->coordinates[axis::Y]));
}

double distance(house* a, workplace* b){
    return  sqrt((a->coordinates[axis::X] - b->coordinates[axis::X])*(a->coordinates[axis::X] - b->coordinates[axis::X])+
            (a->coordinates[axis::Y] - b->coordinates[axis::Y])*(a->coordinates[axis::Y] - b->coordinates[axis::Y]));
}

double distance(house* a, house* b){
    return  sqrt((a->coordinates[axis::X] - b->coordinates[axis::X])*(a->coordinates[axis::X] - b->coordinates[axis::X])+
            (a->coordinates[axis::Y] - b->coordinates[axis::Y])*(a->coordinates[axis::Y] - b->coordinates[axis::Y]));
}


double distance(double* a, double* b){
    return  sqrt((a[axis::X] - b[axis::X])*(a[axis::X] - b[axis::X])+
            (a[axis::Y] - b[axis::Y])*(a[axis::Y] - b[axis::Y]));
}
