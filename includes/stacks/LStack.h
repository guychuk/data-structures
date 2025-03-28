#pragma once

#include "Stack.h"
#include "../lists/linked_lists/SLinkedList.h"
#include <stdexcept>

/** A stack implemented using a singly linked list. */
template<typename T>
class LStack : public Stack<T>
{
public:
    LStack() = default;

    ~LStack() = default;

    bool isEmpty();

    void push(T item);

    T pop();

    T peek();

private:
    SLinkedList<T> stack_;
};

template<typename T>
T LStack<T>::peek()
{
    if (isEmpty())
    {
        return (T)NULL;
    }

    return stack_.get(0);
}

template<typename T>
T LStack<T>::pop()
{
    if (isEmpty())
    {
        throw std::out_of_range("nothing to dequeue");
    }

    T popped = stack_.get(0);

    stack_.remove(popped);

    return popped;
}

template<typename T>
void LStack<T>::push(T item)
{
    stack_.addFirst(item);
}

template<typename T>
bool LStack<T>::isEmpty()
{
    return stack_.isEmpty();
}
