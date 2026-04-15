#ifndef __L_STACK_H__
#define __L_STACK_H__

template <typename T>
class l_stack : public stack<T>{
    private:
    T length;

    public:
    l_stack();

    int64_t get_length();
    void append(T val);
    bool pop();
    void dp();
};

#include "l_stack_definition.h"

#endif //__L_STACK_H__
