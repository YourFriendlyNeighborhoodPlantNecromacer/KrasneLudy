#ifndef __dynamicArray_H__
#define __dynamicArray_H__
#include <inttypes.h>
template <typename T>
class dynamicArray{ /// Requires standarization and bug-squashing
    private:
    int64_t size;
    int64_t lastIndex;
    T* array;

    public:
    dynamicArray();
    dynamicArray(int64_t size);
    dynamicArray(const dynamicArray &other);
    ~dynamicArray();

    dynamicArray& operator = (const dynamicArray &other);
    dynamicArray(dynamicArray&& other) noexcept;
    dynamicArray& operator=(dynamicArray&& other) noexcept;

    dynamicArray operator + (const dynamicArray &other);
    dynamicArray operator + (const T &value) const;
    T& operator [] (const int64_t &index);
    const T& operator [] (const int64_t &index) const;

    bool operator == (const dynamicArray &other);

    bool isEmpty() const;

    int64_t getSize() const;
    int64_t getLastIndex() const;

    bool insert(T&& val, int64_t index);
    void append(T&& val);
    void prepend(T&& val);

    bool insert(const T& val, int64_t index);
    void append(const T& val);
    void prepend(const T& val);

    bool remove(int64_t index);
    void shrinkToFit();
    void forceShrink(int64_t newSize);
    void clear();

    bool swap(int64_t indexA, int64_t indexB);
    void reverseOrder();

    T& at(int64_t index);
    T& front();
    T& back();

    void dp(int64_t tabulation = 0);
};

#include "dynamicArrayDefinition.h"

#endif /// __dynamicArray_H__
