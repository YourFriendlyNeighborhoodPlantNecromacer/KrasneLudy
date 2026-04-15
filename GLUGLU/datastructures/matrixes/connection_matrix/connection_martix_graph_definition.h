#ifndef __CONNECTION_MATRIX_GRAPH__DEFINITION_H__
#define __CONNECTION_MATRIX_GRAPH__DEFINITION_H__
template <typename T>
connection_martix_graph<T>::connection_martix_graph() : size(1), last_index(-1){
    matrix = new T*[1];
    matrix[0] = new T[1];
    matrix[0][0] = false;
}

template <typename T>
connection_martix_graph<T>::connection_martix_graph(int64_t size) : size(size), last_index(-1){
    matrix = new T*[size];
    for(int64_t i=0; i<size; i++) matrix[i] = new T[size]{};
}

template <typename T>
connection_martix_graph<T>::connection_martix_graph(int64_t size, int64_t last_index) : size(size), last_index(last_index){
    matrix = new T*[size];
    for(int64_t i=0; i<size; i++) matrix[i] = new T[size]{};
}

template <typename T>
connection_martix_graph<T>::~connection_martix_graph(){
    std::cout << "DELETING CMG" << std::endl;
    for(int64_t i=0; i<size; i++) delete[] matrix[i];
    delete[] matrix;
    matrix = nullptr;
    std::cout << "deleted CMG" << std::endl;
}

template <typename T>
connection_martix_graph<T>::connection_martix_graph(const connection_martix_graph<T>& other){
    size = other.last_index+1;
    last_index = other.last_index;
    matrix = new T*[size];
    for(int64_t i=0; i<size; i++){
        matrix[i] = new T[size];
        for(int64_t j=0; j<size; j++)
            matrix[i][j] = other.matrix[i][j];
    }
}

template <typename T>
connection_martix_graph<T>& connection_martix_graph<T>::operator=(const connection_martix_graph<T>& other){
    if(this == &other) return *this;
    clear();
    size = other.last_index+1;
    last_index = other.last_index;

    matrix = new T*[size];
    for(int i=0; i<size; i++)matrix[i] = new T[last_index];

    for(int y=0; y<other.size; y++){
        std::cout << "y: " << y << std::endl;
        for(int x=0; x<other.size; x++){
            std::cout << "x: " << x << std::endl;
            matrix[y][x] = other.matrix[y][x];
        }
    }
    return *this;
}

template <typename T>
void connection_martix_graph<T>::insert(){
    last_index++;
    if(last_index == size) expand_martix(matrix, size);
}

template <typename T>
bool connection_martix_graph<T>::remove(int64_t index){
    if(index < 0 || index > last_index) return false;
    for(int64_t j = index; j<last_index; j++){
        for(int64_t i = 0; i< last_index; i++){
            matrix[j][i] = matrix[j+1][i];
            matrix[i][j] = matrix[i][j+1];
        }
        matrix[j][j] = matrix[j+1][j+1];
    }
    for(int64_t i = 0; i<last_index; i++){
        matrix[last_index][i] = matrix[last_index+1][i];
        matrix[i][last_index] = matrix[i][last_index+1];
    }
    matrix[last_index][last_index] = T();
    last_index--;
    return true;
}

template <typename T> ///#TODO
bool connection_martix_graph<T>::clear_vertex(int64_t index){
    std::cout << "inner cleaning in matrix" << std::endl;
    if(index < 0 || index > last_index) return false;
    for(int64_t i = 0; i<=last_index; i++){
        matrix[index][i] = T();
        matrix[i][index] = T();
    }
    std::cout << "ENDING INNER CLEANING IN MATRIX" << std::endl;
    return true;
}

template <typename T>
int64_t connection_martix_graph<T>::get_size(){
    return size;
}

template <typename T>
int64_t connection_martix_graph<T>::get_last_index(){return last_index;}

template <typename T>
T connection_martix_graph<T>::get_data(int64_t from, int64_t to){
    return matrix[from][to];
}

template <typename T>
bool connection_martix_graph<T>::edge_exists(int64_t from, int64_t to){
    if(from > last_index || from < 0 || to > last_index || to < 0) return false;
    return matrix[from][to];
}

template <typename T>
bool connection_martix_graph<T>::make_connection(int64_t from, int64_t to, T value){
    if(from > last_index || from < 0 || to > last_index || to < 0) return false;
    matrix[from][to] = value;
    return true;
}

template <typename T>
bool connection_martix_graph<T>::break_connection(int64_t from, int64_t to){
    if(from > last_index || from < 0 || to > last_index || to < 0) return false;
    if(!matrix[from][to]) return false;
    matrix[from][to] = false;
    return true;
}

template <typename T>
int64_t connection_martix_graph<T>::bfs(int64_t from, int64_t to){
    bool* visited = new bool[size]{false};
    queue<int64_t> to_visit;
    to_visit.append(from);
    visited[from] = false;

    int64_t depth = 1;
    while(!to_visit.is_empty()){
        for(int64_t i=0; i<=last_index; i++){
            if(matrix[to_visit.get_head()->val][i]){
                if(i==to) return depth;
                if(!visited[i]){
                    to_visit.append(i);
                    visited[i] = true;
                }
            }
        }
        to_visit.dequeue();
    }
    return -1;
}

template <typename T>
int64_t connection_martix_graph<T>::rec_dfs(int64_t current_node, int64_t to, int64_t depth, bool* visited){
    visited[current_node] = true;
    for(int64_t i=0; i<=last_index; i++){
        if(matrix[current_node][i]){
            if(i==to){
                return depth+1;
            }
            if(!visited[i]){
                int64_t a = rec_dfs(i, to, depth+1, visited);
                if(a!=-1) return a;
            }
        }
    }
    return -1;
}

template <typename T>
int64_t connection_martix_graph<T>::dfs(int64_t from, int64_t to){
    bool* visited = new bool[last_index]{false};
    int64_t depth = 0;
    int64_t emergency_break = 10;
    int64_t current_node = from;
    return rec_dfs(from, to, depth, visited);
    return -1;
}

template <typename T>
void connection_martix_graph<T>::dp(){
    std::cout << "SIZE: " << size << "   LAST INDEX: " << last_index << std::endl;
    std::cout << "  ";
    for(int64_t i=0; i<size; i++){
        if(i==last_index) std::cout << "$ ";
        else std::cout << i%10 << " ";
    }
    std::cout << std::endl;
    for(int64_t i=0; i<size; i++){
        if(i==last_index) std::cout << "$ ";
        else std::cout << i%10 << " ";
        for(int64_t j=0; j<size; j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
int64_t connection_martix_graph<T>::count_groups(){
    bool* visited = new bool[last_index]{false};
    int64_t group_count=0;
    queue<int64_t> to_visit;
    for(int j = 0; j<=last_index; j++){
        //std::cout << "checking node: " << j << std::endl;
        if(visited[j]) continue;
        group_count++;
        visited[j] = true;
        to_visit.append(j);
        //std::cout << "SAVE AND SOUND!" << std::endl;
        //std::cout << to_visicleart.get_head()->val;
        //std::cout << "SAVE AND SOUND!" << std::endl;
        while(!to_visit.is_empty()){
            for(int64_t i=0; i<=last_index; i++){ /// lesser de nests
                //std::cout << "Looking at " << i << " from " << to_visit.get_head()->val << std::endl;
                if(matrix[to_visit.get_head()->val][i]){
                    //std::cout << "Have we visited: " << i << "? ";
                    if(!visited[i]){
                        //std::cout << "No!" << std::endl;
                        to_visit.append(i);
                        visited[i] = true;
                    } else {
                        //std::cout << "Yes!" << std::endl;
                    }
                }
            }
            to_visit.dequeue();
        }
        //std::cout << "END OF THE LINE" << std::endl;

        /*
        while(!to_visit.is_empty()){
            for(int64_t i=0; i<=last_index; i++){ /// lesser de nests
                if(!matrix[to_visit.get_head()->val][i]) continue;
                if(visited[i]) continue;
                to_visit.append(i);
                visited[i] = true;
            } else {
                    //std::cout << "Yes!" << std::endl;
            }
        to_visit.dequeue();
    }*/
    }
    return group_count;
}

template <typename T>
void connection_martix_graph<T>::clear(){
    if(matrix){
        for(int64_t i=0; i<size; i++) if(!matrix[i]) delete[] matrix[i];
        if(!matrix) delete[] matrix;
        matrix = nullptr;
        size = 0;
        last_index = 0;
    }
}
#endif//__CONNECTION_MATRIX_GRAPH__DEFINITION_H__
