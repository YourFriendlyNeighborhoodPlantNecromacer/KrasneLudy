#ifndef __EXPAND_ARRAY_H__
#define __EXPAND_ARRAY_H__

template <typename T>
void expand_array(T*& array, int64_t& capacity){
    T* new_array = new T[capacity*2];
    for(int64_t i = 0; i<capacity; i++) new_array[i] = array[i];
    capacity *= 2;
    delete[] array;
    array = new_array;
}

#endif//__EXPAND_ARRAY_H__

