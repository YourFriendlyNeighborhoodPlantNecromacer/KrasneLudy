#ifndef __MATRIX_GRAPH_H__
#define __MATRIX_GRAPH_H__

template <typename T>
class matrix_graph{
    private:
    dynamic_array<T> vertecies;
    connection_martix_graph<bool> connection_matrix;
    connection_martix_graph<int64_t> distance_matrix;

    public:
    matrix_graph();
    matrix_graph(int64_t size);
    ~matrix_graph();

    void insert(T val);
    bool remove(int64_t index);
    bool clear_vertex(int64_t index);
    int64_t get_size();
    int64_t get_last_index();
    bool edge_exists(int64_t from, int64_t to);
    bool make_connection(int64_t from, int64_t to, int64_t weight);
    bool break_connection(int64_t from, int64_t to);
    int64_t bfs(int64_t from, int64_t to);
    int64_t dfs(int64_t from, int64_t to);
    int64_t count_groups();
    T& get_vertex(int64_t index);
    void clear();


    dynamic_array<dynamic_array<int64_t>>* find_shortest_paths(int64_t starting_node_index);

    void dp();
    bool construct_from_file(const std::string &file_name);
};


#include "matrix_graph_definition.h"
#endif//__MATRIX_GRAPH_H__
