#ifndef __QUEUE_H__
#define __QUEUE_H__

template <typename T>
class queue{
    private:
    struct node{
        T val;
        node* ahead;

        node();
        node(T val);
        node(T val, node* ahead);
        node(const node &other);
        ~node();

        //the overloaded operators are a code graveyard for now, but they do work if anyone wants to use them.
        node& operator = (const node &other);
        node operator + (const node &other) const;
        node operator - (const node &other) const;
        node& operator += (const node &other);
        node& operator -= (const node &other);
        node operator * (const node &other) const;
        node operator / (const node &other) const;
        node& operator *= (const node &other);
        node& operator /= (const node &other);
        };

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
