#ifndef __COUPLE_DEFINITION_H__
#define __COUPLE_DEFINITION_H__

template <typename T1, typename T2>
couple<T1, T2>::couple() : value_a(), value_b(){}

template <typename T1, typename T2>
couple<T1, T2>::couple(T1 value_a, T2 value_b) : value_a(value_a), value_b(value_b){}



template <typename T1, typename T2>
couple<T1, T2> couple<T1, T2>::operator = (const couple<T1, T2> &other){
    value_a = other.value_a;
    value_b = other.value_b;
    return *this;
}

template<typename T1, typename T2>
std::ostream& operator << (std::ostream& os, const couple<T1, T2>& data){
    os << data.value_a << '_' << data.value_b << std::endl;
    return os;
}
#endif//__COUPLE_DEFINITION_H__
