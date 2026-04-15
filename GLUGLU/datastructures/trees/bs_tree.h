#ifndef __BS_TREE_H__
#define __BS_TREE_H__
template <typename T>
class bs_tree{
    private:
    struct node{
        T val;
        node* left_child;
        node* right_child;

        node();
        node(T val);
        ~node();

        void dp(node* a, int n);
        bool is_leafe();
    };

    node* root;
    public:

    bs_tree();

    bool is_empty();
    node* get_root();
    node* get_min();
    node* get_max();
    node* find(T searched);
    void insert(T value);
    bool remove(T value);
    bool merge();

    void dp();
    void dp_c(node* a, int n);

    void node_insert(T value);

    private:
    bool remove_orphan(node* parent, bool is_left);
    bool remove_single(node* parent, bool is_left);
    bool remove_double(node* parent, bool is_left);
};
#include "bs_tree_definition.h"

#endif //__BS_TREE_H__
