#pragma once

template <typename T>
class Stack
{
public:
    /** Check if the stack is empty. */
    virtual bool isEmpty() = 0;

    /** Insert an element to the stack. */
    virtual void push(T item) = 0;

    /** Remove the last element in the stack. */
    virtual T pop() = 0;

    /** Get the value of the last element in the stack. */
    virtual T peek() = 0;
};

