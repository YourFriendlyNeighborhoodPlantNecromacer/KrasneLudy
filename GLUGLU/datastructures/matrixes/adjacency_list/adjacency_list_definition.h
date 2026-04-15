#ifndef __ADJACENCY_LIST_DEFINITION_H__
#define __ADJACENCY_LIST_DEFINITION_H__

/*
LIST OF LISTS
   LISTS OF CONNECTIONS [basically a single node]
   LISTS OF CONNECTIONS
   ...
   LISTS OF CONNECTIONS

List of locations:
    LOCATION
    LOCATION
    LOCATION
    ...
    LOCATION (polimorphism)
        TYPE
        LOCATION_DATA
        ROADS
            LOCATION* TO
            PROPERTIES
*/

template <typename T>
adjacency_list<T>::adjacency_list() : vertecie_array(){}

template <typename T>
adjacency_list<T>::adjacency_list(int64_t size) : vertecie_array(size){}

template <typename T>
adjacency_list<T>::adjacency_list(const adjacency_list& other) : vertecie_array(other.vertecie_array){}

template <typename T>
adjacency_list<T>::~adjacency_list(){
    std::cout << "See ya next time, space cowboy" << std::endl;
}

template <typename T>
adjacency_list<T>& adjacency_list<T>::operator = (const adjacency_list& other){
    if(this==&other) return *this;
    this->vertecie_array = other.vertecie_array;
    return *this;
}

template <typename T>
bool adjacency_list<T>::is_empty(){
    return vertecie_array.is_empty();
}

template <typename T>
void adjacency_list<T>::insert(T new_vertex){
    vertecie_array.insert(new_vertex, vertecie_array.get_last_index());
}

template <typename T>
bool adjacency_list<T>::remove(int64_t index){
    return false;
}

template <typename T>
int64_t adjacency_list<T>::get_last_index(){
    return vertecie_array.get_last_index();
}

template <typename T>
bool adjacency_list<T>::edge_exists(int64_t from, int64_t to){
    return true; //custom .is_in operation needed. To hell with it making a class for this
}

template <typename T>
bool adjacency_list<T>::make_connection(int64_t from, int64_t to){
    if(from < 0 || to < 0 || from > vertecie_array.get_last_index() || from > vertecie_array.get_last_index()) return false;
    vertecie_array[from].insert(to);
    return true;
}

template <typename T>
bool adjacency_list<T>::break_connection(int64_t from, int64_t to){
    return false;
}

template <typename T>
int64_t adjacency_list<T>::find_path(int64_t from, int64_t to){
    return -1;
}

template <typename T>
int64_t adjacency_list<T>::bfs(int64_t from, int64_t to){
    return -1;
}

template <typename T>
int64_t adjacency_list<T>::dfs(int64_t from, int64_t to){
    return -1;
}

template <typename T>
bool adjacency_list<T>::construct_from_file(const std::string &file_name){
    return false;
}

template <typename T>
void adjacency_list<T>::dp(int64_t tabulation){
    tab(tabulation);
    std::cout << ">>> DEBUG PRINT OF ADJECENCY LIST" << std::endl;
    tab(tabulation);
    std::cout << "SIZE: " << vertecie_array.get_size() << std::endl;
    tab(tabulation);
    std::cout << "LAST INDEX: " << vertecie_array.get_last_index() << std::endl;
    nl();
    tab(tabulation);
    std::cout << "CHILD LISTS: " << std::endl;
    tab(tabulation);
    for(int i=0; i<vertecie_array.get_last_index(); i++){
        tab(tabulation + 3);
        std::cout << "i:" << i << "   vertecie_array[i]:" << vertecie_array[i] << std::endl;
    }
    tab(tabulation);
    std::cout << "<<<" << std::endl;
}
#endif//__ADJACENCY_LIST_DEFINITION_H__
