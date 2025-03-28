#include "Stack.h"
#include <stdexcept>

/** A stack implemented using an array. */
template<typename T>
class AStack : public Stack<T>
{
public:
    /** Create a new stack with a limited number of elements. */
    explicit AStack(int capacity);

    ~AStack();

    bool isEmpty();

    void push(T item);

    T pop();

    T peek();

    /** Get the maximum number of items in the stack. */
    int capacity();

private:
    T* array_;
    int capacity_;  // the max. number of elements
    int size_;      // the actual number of elements
};

template<typename T>
T AStack<T>::peek()
{
    if (isEmpty())
    {
        return NULL;
    }

    return array_[size_ - 1];
}

template<typename T>
T AStack<T>::pop()
{
    if (isEmpty())
    {
        throw std::underflow_error("nothing to pop");
    }

    T popped = array_[size_ - 1];

    size_--;

    return popped;
}

template<typename T>
void AStack<T>::push(T item)
{
    if (size_ == capacity_)
    {
        throw std::overflow_error("the stack is full");
    }

    array_[size_++] = item;
}

template<typename T>
int AStack<T>::capacity()
{
    return capacity_;
}

template<typename T>
bool AStack<T>::isEmpty()
{
    return size_ == 0;
}

template<typename T>
AStack<T>::~AStack()
{
    delete[] array_;
    array_ = nullptr;
}

template<typename T>
AStack<T>::AStack(int capacity)
{
    array_ = new T[capacity];
    capacity_ = capacity;
    size_ = 0;
}
