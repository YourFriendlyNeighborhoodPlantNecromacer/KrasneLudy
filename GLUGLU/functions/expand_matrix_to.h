#ifndef __EXPAND_MATRIX_TO_H__
#define __EXPAND_MATRIX_TO_H__
template <typename T>
void expandMartixTo(T**& matrix, int64_t& size, int64_t aimed_size){
    T** newMatrix = new T*[aimed_size];
    for(int64_t i=0; i<aimed_size; i++) newMatrix[i] = new T[aimed_size];

    for(int64_t i = 0; i<size; i++){
        for(int64_t j = 0; j<size; j++){
            newMatrix[i][j] = matrix[i][j];
        }
    }
    for(int64_t i=0; i<size; i++){
        delete[] matrix[i];
    }
    delete[] matrix;

    size = aimed_size;
    matrix = newMatrix;
}
#endif//__EXPAND_MATRIX_TO_H__
