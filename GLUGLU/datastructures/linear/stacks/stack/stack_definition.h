#ifndef __STACK_DEFINITION_H__
#define __STACK_DEFINITION_H__
#include <iostream>
#include "../../../../functions/tabulator.h"

template <typename T>
stack<T>::node::node() : val(T()), under(nullptr){}
template <typename T>
stack<T>::node::node(T val) : val(val), under(nullptr){}
template <typename T>
stack<T>::node::node(T val, node* under) : val(val), under(under){}
template <typename T>
stack<T>::node::~node(){delete under;}

template <typename T>
stack<T>::stack() : top_node(nullptr), length(0){}
template <typename T>
stack<T>::stack(T val) : top_node(new node(val)), length(1){}
template <typename T>
stack<T>::~stack(){if(!is_empty())delete top_node;}

template <typename T>
bool stack<T>::is_empty(){return length == 0;}
template <typename T>
void stack<T>::append(T val){
    length++;
    node* temp = new node(val, top_node);
    top_node = temp;
}
template <typename T>
bool stack<T>::pop(){
    if(is_empty()) return false;
    length--;
    node* temp = top_node;
    top_node = top_node->under;
    temp->under = nullptr;
    delete temp;
    return true;
}

template <typename T>
void stack<T>::dp(int64_t tabulation){
    std::cout << ">>> DEBUG PRING OF STACK" << std::endl;
    node* temp = top_node;
    std::cout << "LENGTH: " << length << std::endl;
    std::cout << "ELEMENTS:" << std::endl;
    while(temp){
        tab(tabulation+4);
        std::cout << temp->val << std::endl;
        temp = temp->under;
    }
    std::cout << "<<< DEBUG PRING OF STACK" << std::endl;
}

template <typename T>
T stack<T>::top(){
    return top_node->val;
}


#endif // __STACK_DEFFINITION_H__
