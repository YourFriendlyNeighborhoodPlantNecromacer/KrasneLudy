#ifndef __ADJACENCY_LIST_GRAPH_H__
#define __ADJACENCY_LIST_GRAPH_H__

/*
dynamic array:
    - dynamic array
    - dynamic array
    - dynamic array
    ...
    - dynamic array
        couple(weight, id);
Couple: ()

*/

template <typename T>
class adjacency_list{
    private:
    dynamic_array<T> vertecie_array;

    public:
    adjacency_list();
    adjacency_list(int64_t size);
    adjacency_list(const adjacency_list& other);
    ~adjacency_list();

    adjacency_list& operator = (const adjacency_list& other);

    bool is_empty();
    void insert(T new_vertex);
    bool remove(int64_t index);
    int64_t get_last_index();
    bool edge_exists(int64_t from, int64_t to);
    bool make_connection(int64_t from, int64_t to);
    bool break_connection(int64_t from, int64_t to);
    int64_t find_path(int64_t from, int64_t to);
    int64_t bfs(int64_t from, int64_t to);
    int64_t dfs(int64_t from, int64_t to);
    bool construct_from_file(const std::string &file_name);

    void dp(int64_t tabulation = 0);

    private:
    int64_t rec_dfs(int64_t current_node, int64_t to, int64_t depth, bool* visited);

};

#include "adjacency_list_definition.h"

#endif//__ADJACENCY_LIST_H__
