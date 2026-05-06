#ifndef __B_TREE_DEFINITION_H__
#define __B_TREE_DEFINITION_H__

template <typename T>
b_tree<T>::b_tree() : root(nullptr){}

template <typename T>
typename b_tree<T>::node* b_tree<T>::get_root(){return root;}

template <typename T>
void insert(char value){

}

template <typename T>
void b_tree<T>::construct_from_dictionary(dynamic_array<int64_t> dictionary){
    for(int i=0; i<=dictionary.get_last_index(); i++) insert(dictionary[i]);
}


#endif //__B_TREE_DEFINITION_H__
