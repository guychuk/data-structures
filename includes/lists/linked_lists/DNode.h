#pragma once

#include <iostream>

/** A node in a doubly linked list.
 * It has a next, prev and data fields. */
template<typename T>
class DNode
{
public:
    /** Create an empty node. */
    DNode();

    /** Create a node with a given value. */
    explicit DNode(T data);

    ~DNode();

    /** Get the data in the node. */
    T data() const;

    /** Change/set the data in the node. */
    void setData(T data);

    /** Check if the node is initialized. */
    bool isInitialized() const;

    DNode<T>* next;
    DNode<T>* prev;

private:
    T data_;
    bool initialized_;
};

template<typename T>
inline bool DNode<T>::isInitialized() const
{
    return initialized_;
}


template<typename T>
inline void DNode<T>::setData(T data)
{
    data_ = data;
    initialized_ = true;
}

template<typename T>
inline T DNode<T>::data() const
{
    if (!initialized_)
    {
        throw std::logic_error("this node is not initialized");
    }

    return data_;
}

template<typename T>
DNode<T>::~DNode()
{
    next = nullptr;
    prev = nullptr;
}

template<typename T>
DNode<T>::DNode(T data)
 {
    data_ = data;
    next = nullptr;
    prev = nullptr;
    initialized_ = true;
}

template<typename T>
DNode<T>::DNode()
{
    next = nullptr;
    prev = nullptr;
    initialized_ = false;
}
