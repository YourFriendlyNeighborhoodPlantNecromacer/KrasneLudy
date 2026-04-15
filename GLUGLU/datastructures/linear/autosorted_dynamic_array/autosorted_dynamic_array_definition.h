#ifndef __AUTOSORTED_DYNAMIC_ARRAY_DEFFINITION_H__
#define __AUTOSORTED_DYNAMIC_ARRAY_DEFFINITION_H__
#include <iostream>

template <typename T>
autosorted_dynamic_array<T>::autosorted_dynamic_array() : size(1), last_index(0), array(new T[size]){};

template <typename T>
autosorted_dynamic_array<T>::autosorted_dynamic_array(int64_t size) : size(size), last_index(0), array(new T[size]){};

template <typename T>
autosorted_dynamic_array<T>::autosorted_dynamic_array(const autosorted_dynamic_array<T> &other){
    size = other.size;
    last_index = other.last_index;
    array = new T[size];

    for(int i = 0; i < last_index; i++){
        array[i] = other.array[i];
    }
}

template <typename T>
autosorted_dynamic_array<T>::~autosorted_dynamic_array(){

    delete[] array;
    array = nullptr;
}

template <typename T>
autosorted_dynamic_array<T> autosorted_dynamic_array<T>::operator + (const autosorted_dynamic_array<T> &other) const{
    if(this == &other) return *this;
    int64_t length = max(last_index, other.last_index);
    expand_array_to(array, last_index, length);
    last_index = length;
    size = length;

    for(int i = 0; i < last_index; i++){
        array[i] += other.array[i];
    }
    return *this;
}

template <typename T>
T& autosorted_dynamic_array<T>::operator [] (const int64_t &index){
    return array[index];
}

template <typename T>
autosorted_dynamic_array<T>& autosorted_dynamic_array<T>::operator = (const autosorted_dynamic_array<T> &other){
    if(this == &other) return *this;
    clear();
    size = other.size;
    last_index = other.last_index;
    array = new T[size];

    for(int i = 0; i < last_index; i++){
        array[i] = other.array[i];
    }
    return *this;
}

template <typename T>
bool autosorted_dynamic_array<T>::is_empty(){
    return last_index==0;
}

template <typename T>
T& autosorted_dynamic_array<T>::at(int64_t index){
    if(index >= last_index){
        return array[0]; ///CTEL
    }
    return array[index];
}

template <typename T>
bool autosorted_dynamic_array<T>::insert(T val){
    int64_t index;
    int64_t left_index = 0;
    int64_t right_index = last_index;
    while(left_index <= right_index){
        index = (left_index + right_index)/2;
        if(val <= array[index+1] && val >= array[index-1]) break;
        if(val > array[index]){
            right_index = index - 1;
            continue;
        }
        left_index = index + 1;

    }
    if(last_index == size) expand_array(array, size);

    for(int64_t i=last_index; i>=index; i--){
        array[i+1] = array[i];
    }
    array[index] = val;
    last_index++;
    return true;
}

template <typename T>
bool autosorted_dynamic_array<T>::remove(int64_t index){
    if(index < 0 || index > last_index) return false;
    for(int64_t i=index; i<last_index; i++){
        array[i] = array[i+1];
    }
    last_index--;
    return true;
}

template <typename T>
T& autosorted_dynamic_array<T>::front(){
    return array[0];
}

template <typename T>
T& autosorted_dynamic_array<T>::back(){
    return array[last_index-1];
}

template <typename T>
void autosorted_dynamic_array<T>::shrink_to_fit(){
    T* new_array = new T[last_index];
    for(int64_t i = 0; i < last_index; i++) new_array[i] = array[i];
    delete[] array;
    array = new_array;
    size = last_index;
}

template <typename T>
void autosorted_dynamic_array<T>::clear(){
    if(array)delete[] array;
    array=nullptr;
    size = 1;
    last_index = 0;
}

template <typename T>
void autosorted_dynamic_array<T>::dp(){
    std::cout << "Size: " << last_index << std::endl << "Capacity: " << size << std::endl;
    if(!array) std::cout << "NULLPTR ARRAY" << std::endl;
    else for(int64_t i=0; i<last_index; i++)std::cout << "  " << array[i] << std::endl;
}

template <typename T>
int64_t autosorted_dynamic_array<T>::get_last_index(){return last_index;}


#endif //__AUTOSORTED_DYNAMIC_ARRAY_DEFFINITION_H__

