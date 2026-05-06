#ifndef __EXPAND_MATRIX_TO_H__
#define __EXPAND_MATRIX_TO_H__
template <typename T>
void expand_martix_to(T**& matrix, int64_t& size, int64_t aimed_size){
    T** new_matrix = new T*[aimed_size];
    for(int64_t i=0; i<aimed_size; i++) new_matrix[i] = new T[aimed_size];

    for(int64_t i = 0; i<size; i++){
        for(int64_t j = 0; j<size; j++){
            new_matrix[i][j] = matrix[i][j];
        }
    }
    for(int64_t i=0; i<size; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    size = aimed_size;
    matrix = new_matrix;
}
#endif//__EXPAND_MATRIX_TO_H__
