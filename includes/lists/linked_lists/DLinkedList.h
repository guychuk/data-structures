#pragma once

#include "../List.h"
#include "DNode.h"
#include <stdexcept>

/** A doubly linked list. */
template<typename T>
class DLinkedList : public List<T>
{
public:
    /** Create an empty list. */
    DLinkedList();

    ~DLinkedList();

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

    /** Remove a node from the list.
     * @note that here the function assumes that the node is in the list! */
    bool remove(DNode<T>* node);

    bool contains(T item) const;

    DNode<T>* getNode(int index) const;

private:
    DNode<T>* head_;    // this is a sentinel node
};

template<typename T>
bool DLinkedList<T>::remove(DNode<T>* node)
{
    if (this->isEmpty())
    {
        return false;
    }

    // "pull" the node from the list
    node->prev->next = node->next;
    node->next->prev = node->prev;

    delete node;

    this->size_--;

    return true;
}

template<typename T>
bool DLinkedList<T>::contains(T item) const
{
    if (this->isEmpty())
    {
        return false;
    }

    // head's next is not the head itself since the getSize is not zero
    DNode<T>* current = head_->next;

    while (current != head_ && current->data() != item)
    {
        current = current->next;
    }

    return current != head_;
}

template<typename T>
DNode<T>* DLinkedList<T>::getNode(int index) const
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    // head's next is not the head itself since the getSize is not zero
    DNode<T>* curr = head_->next;

    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }

    return curr;
}

template<typename T>
bool DLinkedList<T>::remove(T item)
{
    // the process here is the same as in the method "contains"

    if (this->isEmpty())
    {
        return false;
    }

    // head's next is not the head itself since the getSize is not zero
    DNode<T>* current = head_->next;

    while (current != head_ && current->data() != item)
    {
        current = current->next;
    }

    // here it is different

    // scanned all the elements, did not find the item
    if (current == head_)
    {
        return false;
    }

    return remove(current);
}

template<typename T>
T DLinkedList<T>::removeAt(int index)
{
    DNode<T>* node = getNode(index);

    remove(node);

    return node->data();
}

template<typename T>
T DLinkedList<T>::removeFirst()
{
    if (this->isEmpty())
    {
        throw std::underflow_error("this list is empty");
    }

    T retVal = head_->data();

    remove(head_);

    return retVal;
}

template<typename T>
T DLinkedList<T>::removeLast()
{
    if (this->isEmpty())
    {
        throw std::underflow_error("this list is empty");
    }

    T retVal = head_->prev->data();

    remove(head_->prev);

    return retVal;
}

template<typename T>
T DLinkedList<T>::get(int index) const
{
    DNode<T>* node = getNode(index);

    return node->data();
}

template<typename T>
T DLinkedList<T>::set(int index, T item)
{
    if (index >= this->size_)
    {
        throw std::out_of_range("got illegal index");
    }

    // head's next is not the head itself since the getSize is not zero
    DNode<T>* curr = head_->next;

    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }

    T old = curr->data();

    curr->setData(item);

    return old;
}

template<typename T>
void DLinkedList<T>::addLast(T item)
{
    DNode<T>* newNode = new DNode<T>(item);

    // new node comes right before the head
    newNode->next = head_;

    // new node comes right after the head, too
    if (this->isEmpty())
    {
        newNode->prev = head_;
        head_->next = newNode;
    }
    // new node comes before head but not after
    else
    {
        newNode->prev = head_->prev;
        head_->prev->next = newNode;
    }

    // again, new node comes right before the head
    head_->prev = newNode;

    this->size_++;
}

template<typename T>
void DLinkedList<T>::addFirst(T item)
{
    DNode<T>* newNode = new DNode<T>(item);

    // new node comes right after the head
    newNode->prev = head_;

    // new node comes right before the head, too
    if (this->isEmpty())
    {
        newNode->next = head_;
        head_->prev = newNode;
    }
    // new node comes after head but not before
    else
    {
        newNode->next = head_->next;
        head_->next->prev = newNode;
    }

    // again, new node comes right after the head
    head_->next = newNode;

    this->size_++;
}

template<typename T>
void DLinkedList<T>::add(T item, int index)
{
    // here the index can be exactly the getSize
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
        DNode<T>* curr = head_;
        DNode<T>* newNode = new DNode<T>(item);

        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }

        // curr is now in position index - 1

        newNode->prev = curr;
        newNode->next = curr->next;

        newNode->next->prev = newNode;

        curr->next = newNode;

        this->size_++;
    }
}

template<typename T>
void DLinkedList<T>::add(T item)
{
    addLast(item);
}

template<typename T>
DLinkedList<T>::~DLinkedList()
{
    if (this->size() == 0)
    {
        delete head_;
        head_ = nullptr;
        return;
    }

    // save next, because in deletion we'll lose it
    DNode<T>* current = head_->next, * next;

    while (current != head_)
    {
        next = current->next;
        delete current;
        current = next;
    }

    head_ = nullptr;
}

template<typename T>
DLinkedList<T>::DLinkedList()
{
    head_ = new DNode<T>();

    // here, the initializing of the heads pointers (to itself) is redundant,
    // when adding the first item, the insertion function does that.
}
