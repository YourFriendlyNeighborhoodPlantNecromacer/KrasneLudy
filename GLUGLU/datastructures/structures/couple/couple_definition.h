#ifndef __COUPLE_DEFINITION_H__
#define __COUPLE_DEFINITION_H__

template <typename T1, typename T2>
couple<T1, T2>::couple() : valueA(), valueB(){}

template <typename T1, typename T2>
couple<T1, T2>::couple(T1 valueA, T2 valueB) : valueA(valueA), valueB(valueB){}



template <typename T1, typename T2>
couple<T1, T2> couple<T1, T2>::operator = (const couple<T1, T2> &other){
    valueA = other.valueA;
    valueB = other.valueB;
    return *this;
}

template<typename T1, typename T2>
std::ostream& operator << (std::ostream& os, const couple<T1, T2>& data){
    os << data.valueA << ':' << data.valueB << std::endl;
    return os;
}
#endif//__COUPLE_DEFINITION_H__
