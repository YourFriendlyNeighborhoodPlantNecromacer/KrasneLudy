#ifndef __MATRIX_GRAPH_DEFINITION_H__
#define __MATRIX_GRAPH_DEFINITION_H__

template <typename T>
matrix_graph<T>::matrix_graph() : vertecies(dynamic_array<T>(1)), connection_matrix(int64_t(1)), distance_matrix(int64_t(1)){};

template <typename T>
matrix_graph<T>::matrix_graph(int64_t size) : vertecies(dynamic_array<T>(size)), connection_matrix(size), distance_matrix(size){};

template <typename T>
matrix_graph<T>::~matrix_graph(){}

template <typename T>
void matrix_graph<T>::insert(T val){
    connection_matrix.insert();
    distance_matrix.insert();
    vertecies.append(val);
}

template <typename T>
bool matrix_graph<T>::remove(int64_t index){
    vertecies.remove(index);
    connection_matrix.remove(index);
    distance_matrix.remove(index);
    return true;
}

template <typename T>
bool matrix_graph<T>::clear_vertex(int64_t index){
    std::cout<<"clearing!"<<std::endl;
    if(!connection_matrix.clear_vertex(index)) return false;
    if(!distance_matrix.clear_vertex(index)) return false;
    vertecies.at(index) = T();
    std::cout<<"CLEANING FINISHED!" <<std::endl;
    return true;
}

template <typename T>
int64_t matrix_graph<T>::get_size(){return connection_matrix.get_size();}

template <typename T>
int64_t matrix_graph<T>::get_last_index(){return connection_matrix.get_last_index();}

template <typename T>
bool matrix_graph<T>::edge_exists(int64_t from, int64_t to){return connection_matrix.edge_exists(from, to);}

template <typename T>
bool matrix_graph<T>::make_connection(int64_t from, int64_t to, int64_t weight){
    if(connection_matrix.make_connection(from, to, true)) return distance_matrix.make_connection(from, to, weight);
    return false;
}

template <typename T>
bool matrix_graph<T>::break_connection(int64_t from, int64_t to){
    return connection_matrix.make_connection(from, to, T());
};

/*
template <typename T>
int64_t matrix_graph<T>::bfs(int64_t from, int64_t to);

template <typename T>
int64_t matrix_graph<T>::dfs(int64_t from, int64_t to);

template <typename T>
int64_t matrix_graph<T>::count_groups();
*/

template <typename T>
T& matrix_graph<T>::get_vertex(int64_t index){
    return vertecies.at(index);
}




template <typename T>
dynamic_array<dynamic_array<int64_t>>* matrix_graph<T>::find_shortest_paths(int64_t starting_node_index){

    int64_t n = this->get_size();

    int64_t INF = 99999999999;

    dynamic_array<int64_t> dist(n);
    dynamic_array<int64_t> parent(n);

    for(int64_t i = 0; i < n; i++){
        dist.append(INF);
        parent.append(-1);
    }

    dist.at(starting_node_index) = 0;

    int64_t weight;
    for(int64_t k = 0; k < n - 1; k++){
        for(int64_t u = 0; u < n; u++){
            for(int64_t v = 0; v < n; v++){

                if(!connection_matrix.edge_exists(u,v)) continue;
                weight = distance_matrix.get_data(u,v);

                if(!(dist.at(u) != INF && dist.at(u) + weight < dist.at(v))) continue;
                    dist.at(v) = dist.at(u) + weight;
                    parent.at(v) = u;
                }
            }
        }

    // Optional negative cycle detection
    for(int64_t u = 0; u < n; u++){
        for(int64_t v = 0; v < n; v++){

            if(connection_matrix.edge_exists(u,v)){
                int64_t weight = distance_matrix.get_data(u,v);

                if(dist.at(u) != INF &&
                   dist.at(u) + weight < dist.at(v)){

                    // negative cycle detected
                    return nullptr;
                }
            }
        }
    }

    dynamic_array<dynamic_array<int64_t>>* result = new dynamic_array<dynamic_array<int64_t>>();
    result->append(dist);
    result->append(parent);

    return result;
}

template <typename T>
void matrix_graph<T>::dp(){
    std::cout << "VECTOR:" << std::endl;
    vertecies.dp();
    std::cout << "CONNECTIONS:" << std::endl;
    connection_matrix.dp();
    std::cout << "WEIGHT:" << std::endl;
    distance_matrix.dp();
}

template <typename T>
bool matrix_graph<T>::construct_from_file(const std::string &file_name){
    std::ifstream file(file_name);
    if(!file) return false;
    int amount_of_nodes = 0;
    file >> amount_of_nodes;

    vertecies.clear();
    connection_matrix.clear();
    distance_matrix.clear();

    vertecies = dynamic_array<int64_t>(amount_of_nodes);
    connection_matrix = connection_martix_graph<bool>(amount_of_nodes, amount_of_nodes-1);
    distance_matrix = connection_martix_graph<int64_t>(amount_of_nodes, amount_of_nodes-1);

    int64_t vertecie_value;
    for(int i=0; i<amount_of_nodes; i++){
        file >> vertecie_value;
        vertecies.append(vertecie_value);
    }

    int64_t current_node = 0;
    int64_t to;
    int64_t weight;
    int64_t amount_of_connections;

    while(file>>amount_of_connections)
    {
        for(int i=0; i<amount_of_connections; i++){
            file >> to >> weight;
            connection_matrix.make_connection(current_node, to, true);
            distance_matrix.make_connection(current_node, to, weight);
        }
        current_node++;
    }
    std::cout << "XXXXXXXXXXXXXXXX" << std::endl;
    file.close();
    std::cout << "XXXXXXXXXXXXXXXX" << std::endl;

    return true;
}

#endif//__MATRIX_GRAPH_DEFINITION_H__
