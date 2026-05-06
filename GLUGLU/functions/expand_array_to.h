#ifndef __EXPAND_ARRAY_TO_H__
#define __EXPAND_ARRAY_TO_H__

template <typename T>
bool expand_array_to(T*& array, int64_t& capacity, int aimed_capacity){
    if(aimed_capacity<=capacity)return false;
    T* new_array = new T[aimed_capacity];
    int64_t i;
    for(i = 0; i<capacity; i++) new_array[i] = array[i];
    for(; i<aimed_capacity; i++) new_array[i] = T();
    capacity = aimed_capacity;
    delete[] array;
    array = new_array;
    return true;
}

#endif//__EXPAND_ARRAY_TO_H__
