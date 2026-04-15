#ifndef __COUPLE_H__
#define __COUPLE_H__

template <typename T1, typename T2>
struct couple{
    T1 value_a;
    T2 value_b;

    couple();
    couple(T1 value_a, T2 value_b);

    couple operator = (const couple &other);
};

template<typename T1, typename T2>
std::ostream& operator << (std::ostream& os, const couple<T1, T2>& data);

#include "couple_definition.h"
#endif//__COUPLE_H__


/*
// LIST
//      LIST
//      LIST
//      LIST
//      ...
//      LIST
//          [index, properties] [...] [...] [...]
//                      \
//                      np waga, przepustowość
*/
