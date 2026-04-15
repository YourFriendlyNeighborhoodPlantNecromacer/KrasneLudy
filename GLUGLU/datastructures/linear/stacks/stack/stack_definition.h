#ifndef __STACK_DEFFINITION_H__
#define __STACK_DEFFINITION_H__

template <typename T>
stack<T>::node::node() : val(int64_t()), under(nullptr){}
template <typename T>
stack<T>::node::node(T val) : val(val), under(nullptr){}
template <typename T>
stack<T>::node::node(T val, node* under) : val(val), under(under){}
template <typename T>
stack<T>::node::~node(){delete under;}

template <typename T>
stack<T>::stack() : top(nullptr){};
template <typename T>
stack<T>::~stack(){delete top;};


template <typename T>
bool stack<T>::is_empty(){return top == nullptr;}
template <typename T>
typename stack<T>::node* stack<T>::get_top(){return top;}
template <typename T>
void stack<T>::append(T val){
    node* temp = new node(val, top);
    top = temp;
}
template <typename T>
bool stack<T>::pop(){
    node* temp = top;
    top = top->under;
    temp->under = nullptr;
    delete temp;
    return true;
}

template <typename T>
void stack<T>::dp(){
    node* temp = top;
    while(temp){
        std::cout << temp->val << std::endl;
        temp = temp->under;
    }
}


#endif // __STACK_DEFFINITION_H__
