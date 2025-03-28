#pragma once

#include "../List.h"
#include "SNode.h"
#include <stdexcept>

/** A singly linked list. */
template<typename T>
class SLinkedList : public List<T>
{
public:
    /** Create an empty list. */
    SLinkedList();

    ~SLinkedList();

    /** Add an item to the list.
     * @note that here the function inserts at the end (calls addLast). */
    void add(T item);

    void add(T item, int index);

    /** Insert an element at the beginning of the list. */
    void addFirst(T item);

    /** Insert an element at the end of the list. */
    void addLast(T item);

    T set(int index, T item);

    T get(int index) const;

    bool remove(T item);

    T removeAt(int index);

    T removeFirst();

    T removeLast();

    bool contains(T item) const;

private:
    SNode<T>* head_;    // this is a sentinel node
    SNode<T>* tail_;
};

template<typename T>
bool SLinkedList<T>::contains(T item) const
{
    if (this->isEmpty())
    {
        return false;
    }

    // head's next is not the head itself since the getSize is not zero
    SNode<T>* current = head_->next;

    while (current != head_ && current->data() != item)
    {
        current = current->next;
    }

    return current != head_;
}

template<typename T>
bool SLinkedList<T>::remove(T item)
{
    // the process here is the same as in the method "contains".
    // the difference is that we save/remember the node before the node we want to remove.

    if (this->isEmpty())
    {
        return false;
    }

    SNode<T>* current = head_->next, * prev = head_;

    while (current != head_ && current->data() != item)
    {
        prev = current;
        current = current->next;
    }

    // scanned all the elements, did not find the item
    if (current == head_)
    {
        return false;
    }

    // make the previous skip its next (current), then remove it
    prev->next = current->next;

    if (current == tail_)
    {
        tail_ = prev;
    }

    delete current;

    this->size_--;

    return true;
}

template<typename T>
T SLinkedList<T>::removeAt(int index)
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    SNode<T>* current = head_;
    T val;

    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }

    val = current->data();

    current->next = current->next->next;
    this->size_--;

    return val;
}

template<typename T>
T SLinkedList<T>::removeFirst()
{
    if (this->isEmpty())
    {
        throw std::underflow_error("this list is empty");
    }

    SNode<T>* first = head_->next;

    head_->next = first->next;

    this->size_--;

    return first->data();
}

template<typename T>
T SLinkedList<T>::removeLast()
{
    return removeAt(this->size_ - 1);
}

template<typename T>
T SLinkedList<T>::get(int index) const
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    // head's next is not the head itself since the getSize is not zero
    SNode<T>* curr = head_->next;

    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }

    return curr->data();
}

template<typename T>
T SLinkedList<T>::set(int index, T item)
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    // head's next is not the head itself since the getSize is not zero
    SNode<T>* curr = head_->next;
    T old;

    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }

    old = curr->data();

    curr->setData(item);

    return old;
}

template<typename T>
void SLinkedList<T>::addLast(T item)
{
    SNode<T>* newNode = new SNode<T>(item);
    newNode->next = head_;

    tail_->next = newNode;  // it changes the head too! they're the same
    tail_ = newNode;

    this->size_++;
}

template<typename T>
void SLinkedList<T>::addFirst(T item)
{
    SNode<T>* newNode = new SNode<T>(item);
    newNode->next = head_->next;

    head_->next = newNode;  // it changes the tail too!

    this->size_++;

    if (this->size_ == 1)
    {
        tail_ = newNode;
    }
}

template<typename T>
void SLinkedList<T>::add(T item, int index)
{
    if (index > this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    if (index == this->size_)
    {
        addLast(item);
    }
    else if (index == 0)
    {
        addFirst(item);
    }
    else
    {
        SNode<T>* curr = head_;
        SNode<T>* newNode = new SNode<T>(item);

        for (int i = 0; i < index - 1; i++)
        {
            curr = curr->next;
        }

        // curr is now the node at position index - 1

        newNode->next = curr->next;

        curr->next = newNode;

        this->size_++;
    }
}

template<typename T>
void SLinkedList<T>::add(T item)
{
    addLast(item);
}

template<typename T>
SLinkedList<T>::~SLinkedList()
{
    // save next, because in deletion we'll lose it
    SNode<T>* current = head_, * next;

    while (current != head_)
    {
        next = current->next;
        delete current;
        current = next;
    }

    head_ = nullptr;
    tail_ = nullptr;
}

template<typename T>
SLinkedList<T>::SLinkedList()
{
    head_ = new SNode<T>();
    tail_ = head_;
    head_->next = head_;
}
