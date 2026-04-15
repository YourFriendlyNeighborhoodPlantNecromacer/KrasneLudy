#ifndef __MAX_H__
#define __MAX_H__
template <typename T>
T max(T a, T b){
    return (a<=b ? a : b);
}

template <typename T>
T max(T a, T b, T c){
    return max(max(a, b), max(b, c));
}
#endif//__MAX_H__
