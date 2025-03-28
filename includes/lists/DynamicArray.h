#pragma once

#include "List.h"
#include <stdexcept>

template<typename T>
class DynamicArray : public List<T>
{
public:
    DynamicArray();

    DynamicArray(T* arr, int len);

    explicit DynamicArray(int initCapacity);

    ~DynamicArray();

    /** get the capacity of the array (not its getSize!) */
    int capacity()
    {
        return capacity_;
    };

    /** add an item to the list. */
    void add(T item);

    /** add an item to the list in a specific index. */
    void add(T item, int index);

    /** set the item in a given index to a new value. returns the old value. */
    T set(int index, T item);

    /** get the item in the given index. */
    T get(int index) const;

    /** get a reference to the item in the given index. */
    T& operator[](int index);

    /** remove an item from the list. */
    bool remove(T item);

    T removeAt(int index);

    T removeFirst();

    T removeLast();

    /** reset the array, shrink to default size if needed. */
    void clear(bool shrink = true);

    /** check if an item is in the list. */
    bool contains(T item) const;

    template <typename E> friend class Sort;

private:
    int capacity_;
    T* array_;
    const int DEFAULT_CAPACITY = 16;
    const float GROWTH_FACTOR = 1.5;

    /** expand the array. */
    void expand();
};

template<typename T>
void DynamicArray<T>::clear(bool shrink)
{
    int oldCapacity = capacity_;

    if (shrink)
    {
        capacity_ = DEFAULT_CAPACITY;
    }
    else
    {
        capacity_ = oldCapacity;
    }

    delete array_;
    array_ = new T[capacity_];
    this->size_ = 0;
}

template<typename T>
T& DynamicArray<T>::operator[](int index)
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    return array_[index];
}

template<typename T>
bool DynamicArray<T>::contains(T item) const
{
    for (int i = 0; i < this->size_; i++)
    {
        if (array_[i] == item)
        {
            return true;
        }
    }

    return false;
}

template<typename T>
bool DynamicArray<T>::remove(T item)
{
    int i = 0;

    while (i < this->size_ && array_[i] != item)
    {
        i++;
    }

    // the item is not in the array
    if (i == this->size_)
    {
        return false;
    }

    // else - found the item in index i

    removeAt(i);

    return true;
}

template<typename T>
T DynamicArray<T>::removeAt(int index)
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    T val = array_[index];

    while (index < this->size_ - 1)
    {
        // "pull" the next elements
        array_[index] = array_[index + 1];
        index++;
    }

    this->size_--;
   
    return val;
}

template<typename T>
T DynamicArray<T>::removeFirst()
{
    return removeAt(0);
}

template<typename T>
T DynamicArray<T>::removeLast()
{
    if (this->isEmpty())
    {
        throw std::underflow_error("this list is empty");
    }

    T val = array_[this->size_-- - 1];

    return val;
}

template<typename T>
T DynamicArray<T>::get(int index) const
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    return array_[index];
}

template<typename T>
T DynamicArray<T>::set(int index, T item)
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    T old = array_[index];
    array_[index] = item;
    return old;
}

template<typename T>
void DynamicArray<T>::add(T item, int index)
{
    if (index > this->size_)
    {
        throw std::out_of_range("cannot insert out of the array");
    }

    if (this->size_ == capacity_)
    {
        expand();
    }

    for (int i = this->size_; i != index - 1; i--)
    {
        array_[i] = array_[i - 1];
    }

    array_[index] = item;
    this->size_++;
}

template<typename T>
void DynamicArray<T>::add(T item)
{
    if (this->size_ == capacity_)
    {
        expand();
    }

    array_[this->size_] = item;
    this->size_++;
}

template<typename T>
void DynamicArray<T>::expand()
{
    capacity_ *= GROWTH_FACTOR;

    T* newArray = new T[capacity_];

    for (int i = 0; i < this->size_; i++)
    {
        newArray[i] = array_[i];
    }

    delete[] array_;

    array_ = newArray;
}

template<typename T>
DynamicArray<T>::~DynamicArray()
{
    delete array_;
    array_ = nullptr;
    capacity_ = 0;
    this->size_ = 0;
}

template<typename T>
DynamicArray<T>::DynamicArray(int initCapacity)
{
    this->size_ = 0;
    capacity_ = initCapacity;
    array_ = new T[capacity_];
}

template<typename T>
DynamicArray<T>::DynamicArray()
{
    this->size_ = 0;
    capacity_ = DEFAULT_CAPACITY;
    array_ = new T[capacity_];
}

template<typename T>
inline DynamicArray<T>::DynamicArray(T* arr, int len)
{
    this->size_ = len;
    capacity_ = len;
    array_ = new T[len];

    for (int i = 0; i < len; i++)
    {
        array_[i] = arr[i];
    }
}
