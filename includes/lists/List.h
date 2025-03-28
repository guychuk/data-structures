#pragma once

template<typename T>
class List
{
public:
    /** Create an empty list. */
    List();

    /** Get the getSize (= number of elements) of a list. */
    virtual int size() const;

    /** Add an item to the list. */
    virtual void add(T item) = 0;

    /** Add an item to the list in a specific index. */
    virtual void add(T item, int index) = 0;

    /** Set the item in a given index to a new value.
     * @return the old value. */
    virtual T set(int index, T item) = 0;

    /** Get the item in the given index. */
    virtual T get(int index) const = 0;

    /** Remove an item from the list.
     * @return true iff the item was removed, false otherwise. */
    virtual bool remove(T item) = 0;

    virtual T removeAt(int index) = 0;

    virtual T removeFirst() = 0;

    virtual T removeLast() = 0;

    /** Check if an item is in the list. */
    virtual bool contains(T item) const = 0;

    /** Check if the list is empty. */
    virtual bool isEmpty() const;

protected:
    int size_;
};

template<typename T>
List<T>::List()
{
    size_ = 0;
}

template<typename T>
inline int List<T>::size() const
{
    return size_;
}

template<typename T>
inline bool List<T>::isEmpty() const
{
    return size_ == 0;
}
