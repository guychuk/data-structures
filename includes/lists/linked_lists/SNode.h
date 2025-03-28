#pragma once

#include <stdexcept>

/** A node in a singly linked list.
 * It has a next and data fields. */
template<typename T>
class SNode
{
public:
    /** Create an empty node. */
    SNode();

    /** Create a node with a given value. */
    explicit SNode(T data);

    ~SNode();

    /** Get the data in the node. */
    T data() const;

    /** Change/set the data in the node. */
    void setData(T data);

    /** Check if the node is initialized. */
    bool isInitialized() const;

    SNode<T>* next;

private:
    T data_;
    bool initialized_;
};

template<typename T>
inline bool SNode<T>::isInitialized() const
{
    return initialized_;
}

template<typename T>
inline void SNode<T>::setData(T data)
{
    data_ = data;
    initialized_ = true;
}

template<typename T>
inline T SNode<T>::data() const
{
    if (!initialized_)
    {
        throw std::logic_error("this node is not initialized");
    }

    return data_;
}

template<typename T>
SNode<T>::~SNode()
{
    next = nullptr;
    initialized_ = false;
}

template<typename T>
SNode<T>::SNode(T data)
{
    next = nullptr;
    data_ = data;
    initialized_ = true;
}

template<typename T>
SNode<T>::SNode()
{
    next = nullptr;
    initialized_ = false;
}
