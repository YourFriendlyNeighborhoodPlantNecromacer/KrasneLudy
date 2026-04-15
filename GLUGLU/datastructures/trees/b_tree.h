#ifndef __B_TREE_H__
#define __B_TREE_H__

template <typename T>
class b_tree{
    private:
    struct node{
        T val;
        node* left_child;
        node* right_child;

        bool is_leaf();
    };

    node* root;
    public:
    node* get_root();

    node* find();
    void insert();
    bool remove();
    bool merge();
};

#include "b_tree_definition.h"

#endif //__B_TREE_H__
