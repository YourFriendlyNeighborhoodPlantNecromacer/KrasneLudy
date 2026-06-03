#ifndef __DYNAMIC_ARRAY_DEFINITION_H__
#define __DYNAMIC_ARRAY_DEFINITION_H__
#include <iostream>
#include <stdexcept>
#include <utility>
#include "../../../functions/expand_array.h"
#include "../../../functions/max.h"
#include "../../../functions/tabulator.h"
#include <type_traits>
template <typename T>
dynamic_array<T>::dynamic_array() : size(1), last_index(-1), array(new T[size]){};

template <typename T>
dynamic_array<T>::dynamic_array(int64_t size) : size(size), last_index(-1), array(new T[size]){};

template <typename T>
dynamic_array<T>::dynamic_array(const dynamic_array<T> &other){
    static_assert(
        std::is_copy_constructible<T>::value,
        "dynamic_array<T>: T must be copy constructible"
    );
    size = other.size;
    last_index = other.last_index;
    array = new T[size];

    for(int i = 0; i <= last_index; i++){
        array[i] = other.array[i];
    }
}

template<typename T>
dynamic_array<T>& dynamic_array<T>::operator=(dynamic_array&& other) noexcept
{
    if (this == &other)
        return *this;

    delete[] array;

    size = other.size;
    last_index = other.last_index;
    array = other.array;

    other.array = nullptr;
    other.size = 0;
    other.last_index = -1;

    return *this;
}

template<typename T>
dynamic_array<T>::dynamic_array(dynamic_array&& other) noexcept
    : size(other.size),
      last_index(other.last_index),
      array(other.array)
{
    other.array = nullptr;
    other.size = 0;
    other.last_index = -1;
}

template <typename T>
dynamic_array<T>::~dynamic_array(){

    delete[] array;
    array = nullptr;
}

template <typename T>
dynamic_array<T> dynamic_array<T>::operator + (const dynamic_array<T> &other) {
    if(this == &other) return *this;
    int64_t length = max(last_index, other.last_index);
    expand_array_to(array, last_index+1, length);
    last_index = length;
    size = length;

    for(int i = 0; i <= last_index; i++){
        array[i] += other.array[i];
    }
    return *this;
}

template <typename T>
T& dynamic_array<T>::operator [] (const int64_t &index){
    return array[index];
}

template <typename T>
const T& dynamic_array<T>::operator [] (const int64_t &index) const{
    return array[index];
}

template <typename T>
dynamic_array<T>& dynamic_array<T>::operator = (const dynamic_array<T> &other){
    static_assert(
        std::is_copy_constructible<T>::value,
        "dynamic_array<T>: T must be copy constructible"
    );

    if(this == &other) return *this;

    clear();
    size = other.size;
    last_index = other.last_index;
    array = new T[size];

    for(int i = 0; i <= last_index; i++){
        array[i] = other.array[i];
    }
    return *this;
}

template <typename T>
bool dynamic_array<T>::is_empty() const{
    return last_index==-1;
}

template <typename T>
T& dynamic_array<T>::at(int64_t index){
    if(index > last_index){
        throw std::out_of_range("Index out of bounds");
    }
    return array[index];
}

template <typename T>
bool dynamic_array<T>::swap(int64_t index_a, int64_t index_b){
    if(index_a < 0 || index_a > last_index || index_b < 0 || index_b > last_index) return false;
    T temp = std::move(array[index_a]);
    array[index_a] = std::move(array[index_b]);
    array[index_b] = std::move(temp);
    return true;
}

template <typename T>
void dynamic_array<T>::reverse_order(){
    T temp;
    for(int64_t i=0; i <= last_index/2; i++){
        temp = std::move(array[i]);
        array[i] = std::move(array[last_index-i]);
        array[last_index-i] = std::move(temp);
    }
}

template <typename T>
bool dynamic_array<T>::insert(T&& val, int64_t index){ /// Consider if adding boundary checks would be good. Propably
    if(index<0){return false;}
    while(index >= size) expand_array(array, size);
    if(index > last_index){
        last_index = index; /// reverse <->
        array[index] = std::move(val);
        return true;
    }

    if(last_index+1 == size) expand_array(array, size);

    for(int64_t i=last_index; i>index; i--){
        array[i] = std::move(array[i-1]);
    }
    array[index] = std::move(val);
    last_index++;
    return true;
}

template <typename T>
void dynamic_array<T>::append(T&& val){
    insert(std::move(val), last_index+1);
}

template <typename T>
void dynamic_array<T>::prepend(T&& val){
    insert(std::move(val), 0);
}

template <typename T>
bool dynamic_array<T>::insert(const T& val, int64_t index){
    if(index<0){return false;}
    while(index >= size) expand_array(array, size);
    if(index > last_index){
        last_index = index; /// reverse <->
        array[index] = std::move(val);
        return true;
    }

    if(last_index+1 == size) expand_array(array, size);

    for(int64_t i=last_index; i>index; i--){
        array[i] = std::move(array[i-1]);
    }
    array[index] = std::move(val);
    last_index++;
    return true;
}



template <typename T>
void dynamic_array<T>::append(const T& val){
    insert(std::move(val), last_index+1);
}

template <typename T>
void dynamic_array<T>::prepend(const T& val){
    insert(std::move(val), 0);
}


template <typename T>
bool dynamic_array<T>::remove(int64_t index){
    if(index < 0 || index > last_index) return false;
    for(int64_t i=index; i<last_index; i++){
        array[i] = std::move(array[i+1]);
    }
    last_index--;
    return true;
}

template <typename T>
T& dynamic_array<T>::front(){
    return array[0];
}

template <typename T>
T& dynamic_array<T>::back(){
    return array[last_index];
}

template <typename T>
void dynamic_array<T>::shrink_to_fit(){
    T* new_array = new T[last_index+1];
    for(int64_t i = 0; i <= last_index; i++) new_array[i] = std::move(array[i]);
    delete[] array;
    array = new_array;
    size = last_index;
}

template <typename T>
void dynamic_array<T>::force_shrink(int64_t new_size){
    T* new_array = new T[new_size];
    for(int64_t i = 0; i < new_size; i++) new_array[i] = std::move(array[i]);
    delete[] array;

    size = new_size;
    if (new_size < last_index) last_index = new_size-1;
    array = new_array;
}

template <typename T>
void dynamic_array<T>::clear(){
    if(array)delete[] array;
    size = 1;
    last_index = -1;
    array = new T[size];
}

template <typename T>
void dynamic_array<T>::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF A DYNAMIC ARRAY" << std::endl;
    tab(tabulation);
    std::cout << "LAST INDEX: " << last_index << std::endl;
    tab(tabulation);
    std::cout << "SIZE: " << size << std::endl;
    tab(tabulation);
    if(!array){
        std::cout << "NULLPTR ARRAY" << std::endl;
    }
    else {
        std::cout << "ELEMENTS: " << std::endl;
        for(int64_t i=0; i<=last_index; i++){
            tab(tabulation + 3);
            std::cout << "  " << array[i] << std::endl;
        }
    }
    tab(tabulation);
    std::cout << "<<<" << std::endl;
}

template <typename T>
int64_t dynamic_array<T>::get_size() const{return size;}


template <typename T>
int64_t dynamic_array<T>::get_last_index() const{return last_index;}


#endif //__DYNAMIC_ARRAY_DEFFINITION_H__
