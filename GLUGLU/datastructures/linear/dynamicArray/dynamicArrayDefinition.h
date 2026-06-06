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
dynamicArray<T>::dynamicArray() : size(1), lastIndex(-1), array(new T[size]){};

template <typename T>
dynamicArray<T>::dynamicArray(int64_t size) : size(size), lastIndex(-1), array(new T[size]){};

template <typename T>
dynamicArray<T>::dynamicArray(const dynamicArray<T> &other){
    static_assert(
        std::is_copy_constructible<T>::value,
        "dynamicArray<T>: T must be copy constructible"
    );
    size = other.size;
    lastIndex = other.lastIndex;
    array = new T[size];

    for(int i = 0; i <= lastIndex; i++){
        array[i] = other.array[i];
    }
}

template<typename T>
dynamicArray<T>& dynamicArray<T>::operator=(dynamicArray&& other) noexcept
{
    if (this == &other)
        return *this;

    delete[] array;

    size = other.size;
    lastIndex = other.lastIndex;
    array = other.array;

    other.array = nullptr;
    other.size = 0;
    other.lastIndex = -1;

    return *this;
}

template<typename T>
dynamicArray<T>::dynamicArray(dynamicArray&& other) noexcept
    : size(other.size),
      lastIndex(other.lastIndex),
      array(other.array)
{
    other.array = nullptr;
    other.size = 0;
    other.lastIndex = -1;
}

template <typename T>
dynamicArray<T>::~dynamicArray(){

    delete[] array;
    array = nullptr;
}

template <typename T>
dynamicArray<T> dynamicArray<T>::operator + (const dynamicArray<T> &other) {
    if(this == &other) return *this;
    int64_t length = max(lastIndex, other.lastIndex);
    expandArrayTo(array, lastIndex+1, length);
    lastIndex = length;
    size = length;

    for(int i = 0; i <= lastIndex; i++){
        array[i] += other.array[i];
    }
    return *this;
}

template <typename T>
T& dynamicArray<T>::operator [] (const int64_t &index){
    return array[index];
}

template <typename T>
const T& dynamicArray<T>::operator [] (const int64_t &index) const{
    return array[index];
}

template <typename T>
dynamicArray<T>& dynamicArray<T>::operator = (const dynamicArray<T> &other){
    static_assert(
        std::is_copy_constructible<T>::value,
        "dynamicArray<T>: T must be copy constructible"
    );

    if(this == &other) return *this;

    clear();
    size = other.size;
    lastIndex = other.lastIndex;
    array = new T[size];

    for(int i = 0; i <= lastIndex; i++){
        array[i] = other.array[i];
    }
    return *this;
}

template <typename T>
bool dynamicArray<T>::isEmpty() const{
    return lastIndex==-1;
}

template <typename T>
T& dynamicArray<T>::at(int64_t index){
    if(index > lastIndex){
        throw std::out_of_range("Index out of bounds");
    }
    return array[index];
}

template <typename T>
bool dynamicArray<T>::swap(int64_t indexA, int64_t indexB){
    if(indexA < 0 || indexA > lastIndex || indexB < 0 || indexB > lastIndex) return false;
    T temp = std::move(array[indexA]);
    array[indexA] = std::move(array[indexB]);
    array[indexB] = std::move(temp);
    return true;
}

template <typename T>
void dynamicArray<T>::reverseOrder(){
    T temp;
    for(int64_t i=0; i <= lastIndex/2; i++){
        temp = std::move(array[i]);
        array[i] = std::move(array[lastIndex-i]);
        array[lastIndex-i] = std::move(temp);
    }
}

template <typename T>
bool dynamicArray<T>::insert(T&& val, int64_t index){ /// Consider if adding boundary checks would be good. Propably
    if(index<0){return false;}
    while(index >= size) expand_array(array, size);
    if(index > lastIndex){
        lastIndex = index; /// reverse <->
        array[index] = std::move(val);
        return true;
    }

    if(lastIndex+1 == size) expand_array(array, size);

    for(int64_t i=lastIndex; i>index; i--){
        array[i] = std::move(array[i-1]);
    }
    array[index] = std::move(val);
    lastIndex++;
    return true;
}

template <typename T>
void dynamicArray<T>::append(T&& val){
    insert(std::move(val), lastIndex+1);
}

template <typename T>
void dynamicArray<T>::prepend(T&& val){
    insert(std::move(val), 0);
}

template <typename T>
bool dynamicArray<T>::insert(const T& val, int64_t index){
    if(index<0){return false;}
    while(index >= size) expand_array(array, size);
    if(index > lastIndex){
        lastIndex = index; /// reverse <->
        array[index] = std::move(val);
        return true;
    }

    if(lastIndex+1 == size) expand_array(array, size);

    for(int64_t i=lastIndex; i>index; i--){
        array[i] = std::move(array[i-1]);
    }
    array[index] = std::move(val);
    lastIndex++;
    return true;
}



template <typename T>
void dynamicArray<T>::append(const T& val){
    insert(std::move(val), lastIndex+1);
}

template <typename T>
void dynamicArray<T>::prepend(const T& val){
    insert(std::move(val), 0);
}


template <typename T>
bool dynamicArray<T>::remove(int64_t index){
    if(index < 0 || index > lastIndex) return false;
    for(int64_t i=index; i<lastIndex; i++){
        array[i] = std::move(array[i+1]);
    }
    lastIndex--;
    return true;
}

template <typename T>
T& dynamicArray<T>::front(){
    return array[0];
}

template <typename T>
T& dynamicArray<T>::back(){
    return array[lastIndex];
}

template <typename T>
void dynamicArray<T>::shrinkToFit(){
    T* newArray = new T[lastIndex+1];
    for(int64_t i = 0; i <= lastIndex; i++) newArray[i] = std::move(array[i]);
    delete[] array;
    array = newArray;
    size = lastIndex;
}

template <typename T>
void dynamicArray<T>::forceShrink(int64_t newSize){
    T* newArray = new T[newSize];
    for(int64_t i = 0; i < newSize; i++) newArray[i] = std::move(array[i]);
    delete[] array;

    size = newSize;
    if (newSize < lastIndex) lastIndex = newSize-1;
    array = newArray;
}

template <typename T>
void dynamicArray<T>::clear(){
    if(array)delete[] array;
    size = 1;
    lastIndex = -1;
    array = new T[size];
}

template <typename T>
void dynamicArray<T>::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF A DYNAMIC ARRAY" << std::endl;
    tab(tabulation);
    std::cout << "LAST INDEX: " << lastIndex << std::endl;
    tab(tabulation);
    std::cout << "SIZE: " << size << std::endl;
    tab(tabulation);
    if(!array){
        std::cout << "NULLPTR ARRAY" << std::endl;
    }
    else {
        std::cout << "ELEMENTS: " << std::endl;
        for(int64_t i=0; i<=lastIndex; i++){
            tab(tabulation + namedValues::constant::tabulatorIndent);
            std::cout << "  " << array[i] << std::endl;
        }
    }
    tab(tabulation);
    std::cout << "<<<" << std::endl;
}

template <typename T>
int64_t dynamicArray<T>::getSize() const{return size;}


template <typename T>
int64_t dynamicArray<T>::getLastIndex() const{return lastIndex;}


#endif //__DYNAMIC_ARRAY_DEFFINITION_H__
