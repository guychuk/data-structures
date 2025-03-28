#pragma once

#include "Queue.h"
#include "../lists/linked_lists/SLinkedList.h"

/** A queue implemented using singly linked list. */
template<typename T>
class LQueue : Queue<T>
{
public:
    LQueue() = default;

    ~LQueue() = default;

    bool isEmpty();

    void enqueue(T item);

    T dequeue();

    T peek();

private:
    SLinkedList<T> queue_;
};

template<typename T>
T LQueue<T>::peek()
{
    if (isEmpty())
    {
        throw std::underflow_error("empty queue");
    }

    return queue_.get(0);
}

template<typename T>
T LQueue<T>::dequeue()
{
    if (isEmpty())
    {
        throw std::underflow_error("nothing to dequeue");
    }

    T dequeued = queue_.get(0);
    queue_.remove(dequeued);

    return dequeued;
}

template<typename T>
void LQueue<T>::enqueue(T item)
{
    queue_.addLast(item);
}

template<typename T>
bool LQueue<T>::isEmpty()
{
    return queue_.isEmpty();
}

