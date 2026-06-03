#include <iostream>
#include <inttypes.h>
#include "huffman_code.h"
huffman_b_tree::node() : appearence_count(0), character(' '), left_child(nullptr), right_child(nullptr){}

bool huffman_b_tree::node::is_leaf(){return left_child == nullptr && right_child == nullptr;}

node* get_root(){
    return root
}
    node* find(char value);
    node* find(dynamic_array<bool> directions);
    void insert(char value);
    bool remove();
};


huffman_b_tree::huffman_b_tree() : root(nullptr){

}


template <typename T>
typename b_tree<T>::node* b_tree<T>::get_root(){return root;}

template <typename T>
void insert(char value){

}

template <typename T>
void b_tree<T>::construct_from_dictionary(dynamic_array<int64_t> dictionary){
    for(int i=0; i<=dictionary.get_last_index(); i++) insert(dictionary[i]);
}

/*
FIRST LOOP : COUNTING LETTERS
ex. Text        : aaabacb
ex. Dictionary  : ['a', 'b', 'c']
Tree:
    .
   / \
  a   b
     /
    c
*/


