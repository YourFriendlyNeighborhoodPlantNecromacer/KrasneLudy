#ifndef __MULTIDIM_H__
#define __MULTIDIM_H__

template <typename T>
struct multidim{
    int64_t amount_of_dimentions;
    int64_t* dim_sizes;
    T* data;

    multidim();
    multidim(int64_t amount_of_dimentions);
    multidim(int64_t amount_of_dimentions, int64_t dim_size);
    multidim(int64_t amount_of_dimentions, int64_t* dim_size_array, int64_t array_size);

    void colabse();
    ///void flatten();
    void remove_dimention();
    void resize_dimention();
};

#include "multidim_definition.h"

#endif//__MULTIDIM_H__
