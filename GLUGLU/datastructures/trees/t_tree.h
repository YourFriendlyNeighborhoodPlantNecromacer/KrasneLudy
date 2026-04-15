#ifndef __T_TREE_H__
#define __T_TREE_H__
template <typename T>
class t_tree{
    private:
    struct node{
        T val;
        node* left_child;
        node* middle_child;
        node* right_child;

        bool is_leaf();
        node* find(T searched);
        void insert();
        bool remove();
    };
    node* root;

    public:
    node* get_root();
    node* find();
    void insert();
    bool remove();
    bool merge();
};
#endif//__T_TREE_H__
