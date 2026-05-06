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
    b_tree();

    node* get_root();
    node* find(char value);
    node* find(dynamic_array<bool> directions);
    void insert(char value);
    void construct_from_dictionary(dynamic_array<int64_t> dictionary);
    bool remove();

};

#include "b_tree_definition.h"

#endif //__B_TREE_H__

/*
Ananas-Banan
90           (
                .
              /

      .
     / \
    0   1
   /   /  \
  a   0     1
       \   / \
        1 0   1
         \ \   \
          n s   b

1110000
01011011111100
annbsa
ANNBSA
*/
