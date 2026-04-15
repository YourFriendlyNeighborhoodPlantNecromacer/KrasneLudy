#ifndef __STACK_H__
#define __STACK_H__
#include "iostream"

template <typename T>
class stack{
    private:
    struct node{
        T val;
        node* under;

        node();
        node(T val);
        node(T val, node* under);

        ~node();

        node operator + (node &other);
        node operator - (node &other);
        node operator / (node &other);
        node operator * (node &other);
    };
    node* top;

    public:
    stack();
    ~stack();

    bool is_empty();
    node* get_top();
    void append(T val);
    bool pop();
    void dp();
};

#include "stack_definition.h"

#endif //____STACK_H__
