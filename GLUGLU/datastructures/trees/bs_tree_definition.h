#ifndef __BS_TREE_DEFINITION_H__
#define __BS_TREE_DEFINITION_H__

template <typename T>
bs_tree<T>::node::node() : val(), left_child(nullptr), right_child(nullptr){};

template <typename T>
bs_tree<T>::node::node(T val) : val(val), left_child(nullptr), right_child(nullptr){};

template <typename T>
bs_tree<T>::node::~node(){
    delete left_child;
    delete right_child;
};

template <typename T>
bool bs_tree<T>::node::is_leafe(){return(left_child == nullptr) && (right_child==nullptr);}

template <typename T>
bs_tree<T>::bs_tree() : root(nullptr){}

template <typename T>
bool bs_tree<T>::is_empty(){return root == nullptr;}

template <typename T>
typename bs_tree<T>::node* bs_tree<T>::get_root(){return root;}

template <typename T>
typename bs_tree<T>::node* bs_tree<T>::get_min(){
    if(is_empty()); ///throw exception
    node* temp = root;
    while(temp->left_child){temp = temp->left_child;}
    return temp;
}

template <typename T>
typename bs_tree<T>::node* bs_tree<T>::get_max(){
    if(is_empty()); ///throw exception
    node* temp = root;
    while(temp->right_child){temp = temp->right_child;}
    return temp;
}

template <typename T>
typename bs_tree<T>::node* bs_tree<T>::find(T searched){
    node* temp = root;
    while(root->value != searched){
        if(searched < temp->val){
            if(temp->left_child){
                temp == temp->left_child;
                continue;
            }
            return nullptr;
        }
        if(searched > temp->val){
            if(temp->left_child){
                temp == temp->right_child;
                continue;
            }
            return nullptr;
        } ///CHECK IMPL FILE
        return nullptr;
    }
    return temp;
}


template <typename T>
void bs_tree<T>::insert(T value){
    if(!root){
        root = new node(value);
        return;
    }
    node* temp = root;
    int em_b = 3;
    while(true){
        if(value < temp->val){
            if(temp->left_child){
                temp = temp->left_child;
                continue;
            } else {
                temp->left_child = new node(value);
                return;
            }
        }

        if(temp->right_child){
            temp = temp->right_child;
            continue;
        } else {
            temp->right_child = new node(value);
            return;
        }
    }
}

template <typename T>
bool bs_tree<T>::remove(T value){
    if(is_empty()) return false; ///THROWS A FUSS!
    if(root->is_leafe()){
        if(root->val!=value)return false;
        delete root;
        return true;
    }
    node* parent_of_node_to_remove = root;
    while(true){
        if(parent_of_node_to_remove->left_child) if(parent_of_node_to_remove->left_child->val == value) break;
        if(parent_of_node_to_remove->right_child) if(parent_of_node_to_remove->right_child->val == value) break;

        if(value < parent_of_node_to_remove->val){
            if(parent_of_node_to_remove->left_child){
                parent_of_node_to_remove = parent_of_node_to_remove->left_child;
                continue;
            } else {
                return false;
            }
        }
        if(parent_of_node_to_remove->right_child){
            parent_of_node_to_remove = parent_of_node_to_remove->right_child;
            continue;
        } else {
            return false;
        }
    }
    node* node_to_remove;
    bool is_left;
    if(parent_of_node_to_remove->left_child) if(parent_of_node_to_remove->left_child->val == value){
        node_to_remove = parent_of_node_to_remove->left_child;
        is_left = true;
    }
    if(parent_of_node_to_remove->right_child) if(parent_of_node_to_remove->right_child->val == value){
        node_to_remove = parent_of_node_to_remove->right_child;
        is_left = false;
    }

    if(node_to_remove->is_leafe()) return remove_orphan(parent_of_node_to_remove, is_left);
    if(node_to_remove->left_child && node_to_remove->right_child) return remove_double(parent_of_node_to_remove, is_left);
    remove_single(parent_of_node_to_remove, is_left);
    return true;
}

template <typename T>
bool bs_tree<T>::remove_orphan(node* parent, bool is_left){
    if(is_left == true){
        delete parent->left_child;
        parent->left_child = nullptr;
    } else {
        delete parent->right_child;
        parent->right_child = nullptr;
    }
    return true;
}

template <typename T>
bool bs_tree<T>::remove_single(node* parent, bool is_left){ ///f.b.s.t.  "is_left"
    node* temp;
    std::cout << "STARTING REMOVAL" << std::endl;
    if(is_left == true){
        std::cout << "REMOVING LEFT CHILD" << std::endl;
        temp = parent->left_child;
        if(temp->left_child){
            parent->left_child = temp->left_child;
            temp->left_child= nullptr;
        } else {
            parent->left_child = temp->right_child;
            temp->right_child = nullptr;
        }
    } else {
        std::cout << "REMOVING RIGHT CHILD" << std::endl;
        temp = parent->right_child;
        if(temp->left_child){
            std::cout << "IT HAS CHILD TO THE LEFT" << std::endl;
            parent->right_child  = temp->left_child;
            temp->left_child=nullptr;
        } else {
            std::cout << "IT HAS CHILD ON THE RIGHT" << std::endl;
            parent->right_child = temp->right_child;
            temp->right_child=nullptr;
            //wrong_child
            /// officer i swear it is mine
        }
    }
    delete temp;
    return true;
}

template <typename T>
bool bs_tree<T>::remove_double(node* parent, bool is_left){
    node* temp;
    if(is_left)temp = parent->left_child;
    if(!is_left)temp = parent->right_child;

    node* smallest_large = temp->right_child;
    if(temp->right_child->is_leafe()){
        node* swap = temp;
        temp = temp->right_child;


        return true;
    }
    while(smallest_large->left_child){
        smallest_large = smallest_large->left_child;
    }
    return false;
}



template <typename T>
void bs_tree<T>::node::dp(node* a, int n){
    if(a->left_child){
        a->dp(a->left_child, n+1);
    }
    std::cout << a->val;
    if(a->left_child)std::cout << "   LEFT: " << a->left_child->val;
    if(a->right_child)std::cout << "   RIGHT: " << a->right_child->val;
    std::cout << std::endl;
    if(a->right_child){
        a->dp(a->right_child, n+1);
    }
}

template <typename T>
void bs_tree<T>::dp(){
    if(!root){
        std::cout << "EMPTY" << std::endl;
        return;
    }
    std::cout << "TREE START" << std::endl;
    dp_c(root, 0);
    std::cout << "TREE END" << std::endl;
}

template <typename T>
void bs_tree<T>::dp_c(node* a, int n){
    std::cout << a->val;
    if(a->left_child)std::cout << "   LEFT: " << a->left_child->val;
    if(a->right_child)std::cout << "   RIGHT: " << a->right_child->val;
    std::cout << std::endl;
    if(a->left_child){
        a->dp(a->left_child, n+1);
    }
    if(a->right_child){
        a->dp(a->right_child, n+1);
    }
}


#endif//__BS_TREE_DEFINITION_H__
