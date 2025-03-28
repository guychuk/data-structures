#pragma once

template <typename T>
class Queue
{
public:
    /** Check if the queue is empty. */
    virtual bool isEmpty() = 0;

    /** Insert an element to the queue. */
    virtual void enqueue(T item) = 0;

    /** Remove the first element in the queue. */
    virtual T dequeue() = 0;

    /** Get the value of the first element in the queue. */
    virtual T peek() = 0;
};

