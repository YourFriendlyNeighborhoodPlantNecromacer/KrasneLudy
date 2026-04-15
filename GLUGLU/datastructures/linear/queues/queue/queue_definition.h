#ifndef __QUEUE_DEFFINITION_H__
#define __QUEUE_DEFFINITION_H__
//TO DO: Implement Smart pointers
//       Overload commented out operators

///NODE
template <typename T>
queue<T>::node::node() : val(), ahead(nullptr){};

template <typename T>
queue<T>::node::node(T val) : val(val), ahead(nullptr){};

template <typename T>
queue<T>::node::node(T val, node* ahead) : val(val), ahead(ahead){};

template <typename T>
queue<T>::node::~node(){delete ahead;};

template <typename T>
queue<T>::node::node(const node &other){
    this->val = other.val;
    ahead = nullptr;
}


template <typename T>
typename queue<T>::node& queue<T>::node::operator = (const node &other){
    if(this==&other) return *this;
    this->val = other.val;
    return *this;
}

// This is a code graveyard, I will keep it for now but for now it is pretty much useless
template <typename T>
typename queue<T>::node queue<T>::node::operator + (const node &other) const{
    return node(this->val + other.val);
}

template <typename T>
typename queue<T>::node queue<T>::node::operator - (const node &other) const{
    return node(this->val - other.val);
}

template <typename T>
typename queue<T>::node& queue<T>::node::operator += (const node &other){
    this->val += other.val;
    return *this;
}

template <typename T>
typename queue<T>::node& queue<T>::node::operator -= (const node &other){
    this->val -= other.val;
    return *this;
}

template <typename T>
typename queue<T>::node queue<T>::node::operator * (const node &other) const{
    return node(this->val * other.val);
}

template <typename T>
typename queue<T>::node queue<T>::node::operator / (const node &other) const{
    return node(this->val / other.val);
}

template <typename T>
typename queue<T>::node& queue<T>::node::operator *= (const node &other){
    this->val *= other.val;
    return *this;
}

template <typename T>
typename queue<T>::node& queue<T>::node::operator /= (const node &other){
    this->val /= other.val;
    return *this;
}
//

///QUEUE
template <typename T>
queue<T>::queue() : head(nullptr), rear(nullptr), length(0){};


template <typename T>
queue<T>::queue(const queue &other) : head(nullptr), rear(nullptr), length(0){
    node* temp = other.head;
    while(temp){
        append(temp);
        temp = temp->ahead;
    }
}

template <typename T>
queue<T>::~queue(){delete head;}

template <typename T>
queue<T>& queue<T>::operator = (const queue &other){
    if(this==&other)return *this;
    delete head;
    head = nullptr;
    rear = nullptr;
    length = 0;

    node* temp = other.head;
    while(temp){
        append(temp);
        temp = temp->ahead;
    }
    return *this;
}

template <typename T>
queue<T> queue<T>::operator + (const queue &other) const{
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    queue<T> new_queue;
    while(this_node_ptr && other_node_ptr){
        new_queue.append(this_node_ptr->val + other_node_ptr->val);
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return new_queue;
}

template <typename T>
queue<T> queue<T>::operator - (const queue &other) const{
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    queue<T> new_queue;
    while(this_node_ptr && other_node_ptr){
        new_queue.append(this_node_ptr->val - other_node_ptr->val);
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return new_queue;
}

template <typename T>
queue<T>& queue<T>::operator += (const queue &other){
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    while(this_node_ptr && other_node_ptr){
        this_node_ptr->val += other_node_ptr->val;
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return *this;
}

template <typename T>
queue<T>& queue<T>::operator -= (const queue &other){
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    queue<T> new_queue;
    while(this_node_ptr && other_node_ptr){
        this_node_ptr->val -= other_node_ptr->val;
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return *this;
}


template <typename T>
queue<T> queue<T>::operator * (const queue &other) const{
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    queue<T> new_queue;
    while(this_node_ptr && other_node_ptr){
        new_queue.append(this_node_ptr->val * other_node_ptr->val);
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return new_queue;
}

template <typename T>
queue<T> queue<T>::operator / (const queue &other) const{
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    queue<T> new_queue;
    while(this_node_ptr && other_node_ptr){
        new_queue.append(this_node_ptr->val / other_node_ptr->val);
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return new_queue;
}

template <typename T>
queue<T>& queue<T>::operator *= (const queue &other){
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    while(this_node_ptr && other_node_ptr){
        this_node_ptr->val *= other_node_ptr->val;
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return *this;
}

template <typename T>
queue<T>& queue<T>::operator /= (const queue &other){
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    while(this_node_ptr && other_node_ptr){
        this_node_ptr->val /= other_node_ptr->val;
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    return *this;
}



template <typename T>
bool queue<T>::is_empty(){return length == 0;}

template <typename T>
typename queue<T>::node* queue<T>::get_head(){return head;}

template <typename T>
typename queue<T>::node* queue<T>::get_rear(){return rear;}

template <typename T>
void queue<T>::append(T val){
    node* temp = new node(val);
    length++;
    if(!rear){
        rear = temp;
        head = temp;
    } else {
        rear->ahead = temp;
        rear = temp;
    }
};

template <typename T>
void queue<T>::append(node* val){
    append(val->val);
};

template <typename T>
bool queue<T>::dequeue(){
    if(is_empty()) return false;
    node* temp = head;
    head = head->ahead;
    if(!head) rear = nullptr;
    temp->ahead = nullptr;
    delete temp;
    length--;
    return true;
}

template <typename T>
void queue<T>::merge_with(const queue &other){
    node* this_node_ptr = this->head;
    node* other_node_ptr = other.head;
    while(this_node_ptr && other_node_ptr){
        this_node_ptr->val += other_node_ptr->val;
        this_node_ptr = this_node_ptr->ahead;
        other_node_ptr = other_node_ptr->ahead;
    }
    while(other_node_ptr){
        append(other_node_ptr);
        other_node_ptr = other_node_ptr->ahead;
    }
}


template <typename T>
void queue<T>::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF QUEUE<T>" << std::endl;
    node* temp;
    temp = head;
    if(!temp){
        tab(tabulation);
        std::cout << "THE QUEUE IS EMPTY" << std::endl;
        tab(tabulation);
        std::cout << "<<<" << std::endl;
        return;
    }
    tab(tabulation);
    std::cout << "LENGTH: " << length << std::endl;
    tab(tabulation);
    std::cout << "HEAD: " << head << std::endl;
    tab(tabulation);
    std::cout << "HEAD->VAL: " << head->val << std::endl;
    nl();
    tab(tabulation);
    std::cout << "REAR: " << rear << std::endl;
    tab(tabulation);
    std::cout << "REAR->VAL: " << rear->val << std::endl;
    nl();
    tab(tabulation);
    std::cout << "VALUES: " << std::endl;
    uint64_t index = 0;
    while(temp){
        tab(tabulation);
        std::cout << "   " << "INDEX: " << index << "   temp->val: " << temp->val << "   temp" << temp << std::endl;
        index++;
        temp = temp->ahead;
    }
    tab(tabulation);
    std::cout << "<<<" << std::endl;
}
#endif
