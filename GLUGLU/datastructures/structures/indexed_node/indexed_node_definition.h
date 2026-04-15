#ifndef __INDEXED_NODE_DEFFINITION__
#define __INDEXED_NODE_DEFFINITION__
#include <iostream>


template <typename T>
indexed_node<T>::indexed_node() : index(-1), data(){};
template <typename T>
indexed_node<T>::indexed_node(int64_t index) : index(index), data(){};
template <typename T>
indexed_node<T>::indexed_node(T data) : index(-1), data(data){};
template <typename T>
indexed_node<T>::indexed_node(int64_t index, T data) : index(index), data(data){};

template <typename T>
T indexed_node<T>::operator + (const indexed_node &other){return this->data + other.data;}

template <typename T>
T indexed_node<T>::operator - (const indexed_node &other){return this->data + other.data;}

template <typename T>
T indexed_node<T>::operator * (const indexed_node &other){return this->data * other.data;}

template <typename T>
T indexed_node<T>::operator / (const indexed_node &other){return this->data / other.data;}

template <typename T>
void indexed_node<T>::dp(){
    std::cout << index << "   " << data << std::endl;
}

#endif // __INDEXED_NODE_DEFFINITION__
