#pragma once

#include "Queue.h"
#include <stdexcept>

/** A queue implemented using an array. */
template<typename T>
class AQueue : public Queue<T>
{
public:
    /** Create a new queue with a limited number of elements. */
    explicit AQueue(int capacity);

    ~AQueue();

    bool isEmpty();

    void enqueue(T item);

    T dequeue();

    T peek();

    bool isFull();

    /** Get the maximum number of items in the queue. */
    int capacity();

private:
    T* array_;
    int capacity_;
    int beginning_; // index of the first element in the array
    int end_;       // index of the last element in the array
};

template<typename T>
T AQueue<T>::dequeue()
{
    if (isEmpty())
    {
        throw std::underflow_error("nothing to dequeue");
    }

    T dequeued = array_[beginning_];

    // the mod brings us back if we're at the end
    if (beginning_ == end_)
    {
        beginning_ = -1;
        end_ = -1;
    }
    else
    {
        beginning_ = (beginning_ + 1) % capacity_;
    }

    return dequeued;
}

template<typename T>
T AQueue<T>::peek()
{
    if (isEmpty())
    {
        throw std::underflow_error("empty queue");
    }

    return array_[beginning_];
}

template<typename T>
inline bool AQueue<T>::isFull()
{
    return beginning_ != -1 && (end_ + 1) % capacity_ == beginning_;
}

template<typename T>
AQueue<T>::~AQueue()
{
    delete[] array_;
    array_ = nullptr;
}

template<typename T>
bool AQueue<T>::isEmpty()
{
    return beginning_ == -1;
}

template<typename T>
void AQueue<T>::enqueue(T item)
{
    // the mod brings us back if we're at the end
    int indexToInsert = (end_ + 1) % capacity_;

    if (isFull())
    {
        throw std::overflow_error("the queue is full");
    }

    array_[indexToInsert] = item;
    end_ = indexToInsert;

    if (beginning_ == -1)
    {
        beginning_ = 0;
    }
}

template<typename T>
int AQueue<T>::capacity()
{
    return capacity_;
}

template<typename T>
AQueue<T>::AQueue(int capacity)
{
    array_ = new T[capacity];
    capacity_ = capacity;
    beginning_ = -1;
    end_ = -1;
}
