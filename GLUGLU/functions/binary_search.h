#ifndef __BINARY_SEARCH_H__
#define __BINARY_SEARCH_H__
#include <iostream>

template<typename T>
class autosorted_dynamic_array;

template<typename arr, typename value>
int64_t binary_search(arr data, int64_t left_index, int64_t right_index, value searched_value, bool descending = false){
    uint64_t middle;
    std::cout << "BINARY SEARCH" << std::endl;
    if(descending){
        while(left_index<=right_index){
            middle = (left_index+right_index)/2;
            std::cout << "middle: " << middle << "     left_index: " << left_index << "     right_index: " << right_index << std::endl;
            std::cout << "middle: " << data[middle] << "     left_index: " << data[left_index] << "     right_index: " << data[right_index] << std::endl;
            if(searched_value == data[middle]) return middle;
            if(searched_value > data[middle]){
                std::cout << "FIR" << std::endl;
                right_index = middle - 1;
                continue;
            }
            std::cout << "SEC" << std::endl;
            left_index = middle + 1;
            return -1;
        }
        while(left_index<=right_index){
        middle = (left_index+right_index)/2;
        std::cout << "middle: " << middle << "     left_index: " << left_index << "     right_index: " << right_index << std::endl;
        std::cout << "middle: " << data[middle] << "     left_index: " << data[left_index] << "     right_index: " << data[right_index] << std::endl;
        if(searched_value == data[middle]) return middle;
        if(searched_value < data[middle]){
            std::cout << "FIR" << std::endl;
            right_index = middle - 1;
            continue;
        }
        std::cout << "SEC" << std::endl;
        left_index = middle + 1;
    }
    return -1;
}

#endif//__BINARY_SEARCH_H__
