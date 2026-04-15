#ifndef __L_STACK_DEFFINITION_H__
#include <iostream>

template <typename T>
l_stack<T>::l_stack() : length(0){}

template <typename T>
int64_t l_stack<T>::get_length(){return length;}
template <typename T>
void l_stack<T>::append(T val){
    length++;
    stack<T>::append(val);
}
template <typename T>
bool l_stack<T>::pop(){
    if(length == 0) return false;
    length--;
    return stack<T>::pop();
}
template <typename T>
void l_stack<T>::dp(){
    std::cout << "Lenght: " << length << std::endl;
    stack<T>::dp();
}


#endif // ___L_STACK_DEFFINITION_H__
