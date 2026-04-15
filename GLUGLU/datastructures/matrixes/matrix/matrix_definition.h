#ifndef __MATRIX_DEFINITION_H__
#define __MATRIX_DEFINITION_H__

template <typename T>
matrix<T>::matrix() : size(1){
    data = new T*[size];
    for(int i = 0; i<size; i++){
        data[i] = new T[size];
    }
}

template <typename T>
matrix<T>::matrix(int64_t size) : size(size){
    data = new T*[size]; //
    for(int i = 0; i<size; i++){
        data[i] = new T[size];
    }
}

template <typename T>
matrix<T>::~matrix(){
    for(int i = 0; i<size; i++){
        delete[] data[i];
    }
    delete[] data;
    data = nullptr;
}

template <typename T>
matrix<T> matrix<T>::operator + (const matrix &other) const{
    ///adding differently sized matrixes can be a bit dangerous, but it is "fine" for now
    for(int j=0; j<size; j++){
        for(int i=0; i<size; i++){
            data[j][i] += other.data[j][i];
        }
    }
    return *this;
}

template <typename T>
matrix<T>& matrix<T>::operator = (const matrix &other){
    std::cout << "OH NO!" << std::endl;
    for(int j=0; j<size; j++){
        for(int i=0; i<size; i++){
            data[j][i] = other.data[j][i];
        }
    }
    return *this;
}

template <typename T>
T* matrix<T>::operator [] (const int64_t y){
    return data[y];
}

template <typename T>
void matrix<T>::expand(){
    expand_martix(data, size);
}

template <typename T>
void matrix<T>::expand_to(int64_t aimed_size){
    if(size < aimed_size) return;///throw err;
    expand_martix(data, size, aimed_size);
}

template <typename T>
T& matrix<T>::at(int64_t y, int64_t x){
    return data[y][x];
}


template <typename T>
void matrix<T>::dp(){
    for(int j = 0; j<size; j++){
        for(int i = 0; i<size; i++){
            std::cout << data[j][i] << " ";
        }
        std::cout << std::endl;
    }
}


#endif//__MATRIX_DEFINITION_H__
