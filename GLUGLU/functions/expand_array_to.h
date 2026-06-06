#ifndef __EXPAND_ARRAY_TO_H__
#define __EXPAND_ARRAY_TO_H__

template <typename T>
bool expandArrayTo(T*& array, int64_t& capacity, int aimedCapacity){
    if(aimedCapacity<=capacity)return false;
    T* newArray = new T[aimedCapacity];
    int64_t i;
    for(i = 0; i<capacity; i++) newArray[i] = std::move(array[i]);
    for(; i<aimedCapacity; i++) newArray[i] = T();
    capacity = aimedCapacity;
    delete[] array;
    array = newArray;
    return true;
}


#endif//__EXPAND_ARRAY_TO_H__
