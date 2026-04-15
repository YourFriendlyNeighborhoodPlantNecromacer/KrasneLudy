#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__

template <typename T>
class dynamic_array{ /// Requires standarization and bug-squashing
    private:
    int64_t size;
    int64_t last_index;
    T* array;

    public:
    dynamic_array();
    dynamic_array(int64_t size);
    dynamic_array(const dynamic_array &other);
    ~dynamic_array();

    dynamic_array& operator = (const dynamic_array &other);

    dynamic_array operator + (const dynamic_array &other) const;
    dynamic_array operator + (const T &value) const;
    T& operator [] (const int64_t &index);

    bool operator == (const dynamic_array &other);

    bool is_empty();

    int64_t get_size();
    int64_t get_last_index();

    bool insert(T val, int64_t index);
    void append(T val);
    void prepend(T val);

    bool remove(int64_t index);
    void shrink_to_fit();
    void force_shrink(int64_t new_size);
    void clear();

    void sort_asc();
    void sort_dsc();

    T& at(int64_t index);
    T& front();
    T& back();

    void dp(int64_t tabulation = 0);
};

#include "dynamic_array_definition.h"

#endif /// __DYNAMIC_ARRAY_H__
