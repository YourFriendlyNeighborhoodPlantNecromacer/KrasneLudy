#ifndef __STACK_H__
#define __STACK_H__

template <typename T>
class stack{
    private:
    struct node{
        public:
        node* under;
        T val;

        node();
        node(T val);
        node(T val, node* under);
        ~node();
    };
    int64_t length;
    node* top_node;


    public:
    stack();
    stack(T val);
    ~stack();

    bool is_empty();
    int64_t get_length();
    void append(T val);
    bool pop();
    T top();

    void dp(int64_t tabulation = 0);
};

#include "stack_definition.h"

#endif//__STACK_H__
