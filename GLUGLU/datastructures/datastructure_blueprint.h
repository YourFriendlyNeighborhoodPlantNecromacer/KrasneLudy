#ifndef __QUEUE_H__
#define __QUEUE_H__

template <typename T>
class queue{
    private:
    /*
    //private:
        inner_structures
        struct inner_struct{
            ...
        }

        private variables
        private constructors
    public:
        public variables
        public constructors
        public destructor
        public operator overloading
        public methods
    private:
        private methods
    */
    struct inner_struct{}
    node* head;
    node* rear;
    int64_t length;

    public:
    queue();
    queue(const queue &other);
    ~queue();

    queue& operator = (const queue &other);
    queue operator + (const queue &other) const;
    queue operator - (const queue &other) const;
    queue& operator += (const queue &other);
    queue& operator -= (const queue &other);
    queue operator * (const queue &other) const;
    queue operator / (const queue &other) const;
    queue& operator *= (const queue &other);
    queue& operator /= (const queue &other);

    bool is_empty();
    node* get_head();
    node* get_rear();
    T& at();
    int64_t get_lenght();
    void append(T val);
    void append(node* val);
    bool dequeue();
    void merge_with(const queue& other);
    void dp(int64_t tabulation = 0);
};

#include "queue_definition.h"
#endif // __COUPLE_H__

/*
[head] -> ... -> ... -> [rear]
*/
