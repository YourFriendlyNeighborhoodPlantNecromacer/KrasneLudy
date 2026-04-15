#ifndef __N_TREE_H__
#define __N_TREE_H__

template <typename T>
class n_tree{
    private:
    struct node{
        dynamic_array<T*> children;
        bool is_leaf();
    };
    node* root;
    int amount_of_nodes;

    public:
    n_tree();
    ~n_tree();

    node* get_root();

    node* find();
    void insert();
    bool remove();
    bool merge();

};

#include "n_tree_definition.h"


#endif //__N_TREE_H__
