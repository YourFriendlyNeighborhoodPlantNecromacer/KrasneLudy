#ifndef __EXPAND_MATRIX_H__
#define __EXPAND_MATRIX_H__

template <typename T>
void expand_martix(T**& matrix, int64_t& size){
    T** new_matrix = new T*[size*2];
    for(int64_t i=0; i<size*2; i++) new_matrix[i] = new T[size*2]();

    for(int64_t i = 0; i<size; i++){
        for(int64_t j = 0; j<size; j++){
            new_matrix[i][j] = matrix[i][j];
        }
    }
    for(int64_t i=0; i<size; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    size *= 2;
    matrix = new_matrix;
}
#endif//__EXPAND_MATRIX_H__
