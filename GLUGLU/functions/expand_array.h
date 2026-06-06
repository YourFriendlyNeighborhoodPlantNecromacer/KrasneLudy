#ifndef __EXPAND_ARRAY_H__
#define __EXPAND_ARRAY_H__

#include <utility>

template <typename T>
void expand_array(T*& array, int64_t& capacity){
    T* newArray = new T[capacity*2];
    for(int64_t i = 0; i<capacity; i++) newArray[i] = std::move(array[i]);
    capacity *= 2;
    delete[] array;
    array = newArray;
}

template <typename T>
void expand_array(T*&& array, int64_t& capacity){
    T* newArray = new T[capacity*2];
    for(int64_t i = 0; i<capacity; i++) newArray[i] = std::move(array[i]);
    capacity *= 2;
    delete[] array;
    array = newArray;
}

#endif//__EXPAND_ARRAY_H__

