#ifndef __HUFFMAN_CODE_H__
#define __HUFFMAN_CODE_H__
#include <iostream>
#include <inttypes.h>

class huffman_b_tree{
    private:
    struct node{
        int64_t appearence_count;
        char character;
        node* left_child;
        node* right_child;

        node();
        bool is_leaf();
    };

    node* root;
    public:
    huffman_b_tree();

    node* get_root();
    node* find(char value);
    node* find(dynamic_array<bool> directions);
    void insert(char value);
    bool remove();
};

#endif//__HUFFMAN_CODE_H__
