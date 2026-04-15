#ifndef __INDEXED_NODE__
#define __INDEXED_NODE__

template <typename T>
struct indexed_node{
    int64_t index;
    T data;

    indexed_node();
    indexed_node(int64_t index);
    indexed_node(T data);
    indexed_node(int64_t index, T data);

    T operator + (const indexed_node &other);
    T operator + (const T value);
    T operator - (const indexed_node &other);
    T operator - (const T value);
    T operator * (const indexed_node &other);
    T operator * (const T value);
    T operator / (const indexed_node &other);
    T operator / (const T value);
    T operator += (const indexed_node &other);
    T operator += (const T value);
    T operator -= (const indexed_node &other);
    T operator -= (const T value);

    void dp();
};

#include "indexed_node_definition.h"
#endif //__INDEXED_NODE__
