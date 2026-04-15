#ifndef __MATRIX_H__
#define __MATRIX_H__

template <typename T>
struct matrix{
    private:
    int64_t size;
    T** data;

    public:
    matrix();
    matrix(int64_t size);
    matrix(int64_t size, T defult_value);
    matrix(const matrix &other);
    ~matrix();

    matrix& operator = (const matrix &other);
    matrix operator + (const matrix &other) const; ///variants with T as argument
    matrix operator - (const matrix &other) const;
    matrix operator * (const matrix &other) const;
    matrix operator / (const matrix &other) const;
    matrix& operator += (const matrix &other);
    matrix& operator -= (const matrix &other);
    matrix& operator *= (const matrix &other);
    matrix& operator /= (const matrix &other);
    T* operator [] (const int64_t y);

    void expand();
    void expand_to(int64_t aimed_size);
    T& at(int64_t y, int64_t x);
    void dp();
};


#include "matrix_definition.h"
#endif// __MATRIX_H__
