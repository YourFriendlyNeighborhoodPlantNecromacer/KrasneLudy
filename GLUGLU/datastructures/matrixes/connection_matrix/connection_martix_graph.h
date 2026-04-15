#ifndef __CONNECTION_MATRIX_GRAPH_H__
#define __CONNECTION_MATRIX_GRAPH_H__

template <typename T>
class connection_martix_graph{
    protected:
    int64_t size;
    int64_t last_index;
    T** matrix;

    public:
    connection_martix_graph();
    connection_martix_graph(int64_t size);
    connection_martix_graph(int64_t size, int64_t last_index);
    ~connection_martix_graph();
    connection_martix_graph(const connection_martix_graph<T>& other);

    connection_martix_graph& operator=(const connection_martix_graph& other);

    void insert();
    bool remove(int64_t index);
    bool clear_vertex(int64_t index);
    int64_t get_size();
    int64_t get_last_index();
    T get_data(int64_t from, int64_t to);
    bool edge_exists(int64_t from, int64_t to);
    bool make_connection(int64_t from, int64_t to, T value);
    bool break_connection(int64_t from, int64_t to);
    int64_t bfs(int64_t from, int64_t to);
    int64_t dfs(int64_t from, int64_t to);
    int64_t count_groups();
    void clear();

    void dp();

    private:
    int64_t rec_dfs(int64_t current_node, int64_t to, int64_t depth, bool* visited);
};


#include "connection_martix_graph_definition.h"
#endif//__CONNECTION_MATRIX_GRAPH_H__
