#ifndef __MIN_H__
#define __MIN_H__
template <typename T>
T min(T a, T b){
    return (a<=b ? a : b);
}

template <typename T>
T min(T a, T b, T c){
    return min(min(a, b), min(b, c));
}
#endif//__MIN_H__
