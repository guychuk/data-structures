#pragma once

#include "Tree.h"
#include "../lists/DynamicArray.h"
#include "../queues/LQueue.h"

#include <iostream>

#define NO_SUCH_PARENT "no node with this key in the tree"

template<typename T>
class GeneralTree : public Tree<T>
{
public:
    // Constructors

    /**
     * Create an empty tree.
     */
    GeneralTree();

    /**
     * Create a new tree and set its node's key
     * @param key
     */
    explicit GeneralTree(T key);

    // Destructor

    /**
     * Deletes and resets this node and all of its descendants.
     */
    ~GeneralTree();

    // Tree Operations

    virtual GeneralTree<T>* insert(T key);

    /**
     * Insert a new key to the tree.
     * @param key
     * @param parent the key of the parent node to the new key.
     * @return the newly created node.
     */
    virtual GeneralTree<T>* insert(T key, T parent);

    /**
     * Insert a new key to the tree.
     * @param key
     * @param parent the parent node to the new key.
     * @return the newly created node.
     */
    virtual GeneralTree<T>* insert(T key, GeneralTree<T>* parent);

    /**
     * Insert a node to the tree.
     * @param newNode
     * @param parent the key of the parent node to the new key.
     * @return the node that was inserted to the tree or the parent if the parent was empty.
     */
    virtual GeneralTree<T>* insert(GeneralTree<T>* newNode, GeneralTree<T>* parent);

    virtual bool remove(T key);

    /**
     * Remove a key from the tree.
     * @param key
     * @param promoteAll in case the node to remove has multiple children, indicates if the method promotes all of them.
     * @param toPromote if promoteAll is false, this is the index of a specific child to promote.
     * @return true iff the key was removed.
     */
    virtual bool remove(T key, bool promoteAll, int toPromote);

    virtual GeneralTree<T>* search(T key);

    virtual GeneralTree<T>* lowestCommonAncestor(T key1, T key2);

    // Other operations

    virtual void reset();

    // Tree Traversals

    T* preOrder();

    T* postOrder();

    T* levelOrder();

    // Properties of a BinaryTree / Node

    virtual bool isLeaf();

    virtual int degree();

    // Getters

    int getHeight();

    /**
     * Get a child of this node.
     * @param index the index of the child to get.
     * @return the child at the given index.
     */
    virtual GeneralTree<T>* getChild(int index);

protected:
    DynamicArray<GeneralTree<T>*>* children_;

    /**
     * Remove a child of this node.
     * @param child
     * @return true iff the child was removed.
     */
    bool removeChild(GeneralTree<T>* child);

    /**
     * Add a child node to this node at a specific index.
     * @param child
     * @param index the index of the child to add, use -1 to add last.
     */
    void addChild(GeneralTree<T>* child, int index = -1);

    /**
     * Get the index of a child of this node.
     * @param child
     * @return the index of the child if it is a child of this node, or -1 otherwise.
     */
    int indexOf(GeneralTree<T>* child);

private:
    // Traversals Helpers

    /**
     * A helper method of preOrder(void).
     * This method does the actual traversal and inserts the getKeyAt to the array.
     * @param array an array to store the elements.
     * @param index the first available index in the array.
     * @return the first available index in the array after the current call.
     */
    int preOrder(T* array, int index);

    /**
     * A helper method of postOrder(void).
     * This method does the actual traversal and inserts the getKeyAt to the array.
     * @param array an array to store the elements.
     * @param index the first available index in the array.
     * @return the first available index in the array after the current call.
     */
    int postOrder(T* array, int index);
};

// Constructors

template<typename T>
GeneralTree<T>::GeneralTree()
{
    this->size_ = 0;
    this->children_ = new DynamicArray<GeneralTree<T>*>(2);
    this->parent_ = nullptr;
}

template<typename T>
GeneralTree<T>::GeneralTree(T key) : GeneralTree()
{
    this->size_ = 1;
    this->key_ = key;
}

// Destructor

template<typename T>
GeneralTree<T>::~GeneralTree()
{
    for (int i = 0; i < degree(); i++)
    {
        delete children_->get(i);
    }

    delete this->children_;
    this->children_ = nullptr;
    this->parent_ = nullptr;
    this->size_ = 0;
}

// Tree Operations

template<typename T>
GeneralTree<T>* GeneralTree<T>::insert(T key)
{
    return this->insert(new GeneralTree<T>(key), this);
}

template<typename T>
GeneralTree<T>* GeneralTree<T>::insert(T key, T parent)
{
    GeneralTree<T>* parentNode = this->search(parent);

    if (!parentNode)
    {
        throw std::invalid_argument(NO_SUCH_PARENT);
    }

    return this->insert(key, parentNode);
}

template<typename T>
GeneralTree<T>* GeneralTree<T>::insert(T key, GeneralTree<T>* parent)
{
    if (!parent)
    {
        throw std::invalid_argument(NULL_TREE);
    }

    return this->insert(new GeneralTree<T>(key), parent);
}

template<typename T>
GeneralTree<T>* GeneralTree<T>::insert(GeneralTree<T>* newNode, GeneralTree<T>* parent)
{
    if (parent->isEmpty())
    {
        parent->reset();
        parent->key_ = newNode->getKey();
        parent->size_ = 1;
        return parent;
    }

    parent->addChild(newNode);
    return newNode;
}

template<typename T>
bool GeneralTree<T>::remove(T key)
{
    return remove(key, true, 0);
}

template<typename T>
bool GeneralTree<T>::remove(T key, bool promoteAll, int toPromote)
{
    GeneralTree<T>* toRemove = search(key), * child = nullptr, * parent = nullptr;
    int index;

    if (toRemove->hasParent())
    {
        parent = (GeneralTree<T> *) toRemove->getParent();
        index = parent->indexOf(toRemove);
    }

    if (!toRemove)
    {
        return false;
    }

    if (toRemove->isLeaf())
    {
        // simply remove the isLeaf from the tree

        if (toRemove->hasParent())
        {
            ((GeneralTree<T> *) (toRemove->getParent()))->removeChild(toRemove);
        }

        delete toRemove;
    }
    else if (toRemove->degree() == 1)
    {
        child = toRemove->getChild(0);

        if (parent)
        {
            // add the child as a child of the parent node in the index of the original node

            parent->removeChild(toRemove);
            parent->addChild(child, index);

            // delete the node

            toRemove->reset();
            delete toRemove;
        }
        // removing a root
        else
        {
            // copy the child's key to the root, then add the child's children to the root

            toRemove->key_ = child->getKey();
            toRemove->removeChild(child);

            for (int i = 0; i < child->degree(); i++)
            {
                toRemove->addChild(child->getChild(i));
            }

            child->reset();
            delete child;
        }
    }
    else if (promoteAll)
    {
        if (parent)
        {
            parent->removeChild(toRemove);

            // add the children to the parent according to their original order
            for (int i = 0; i < toRemove->degree(); i++)
            {
                parent->addChild(toRemove->getChild(i), index + i);
            }

            toRemove->reset();
            delete toRemove;
        }
        else
        {
            throw std::logic_error("cannot promote all children if this is the node!");
        }
    }
    // promote a specific child
    else
    {
        if (toPromote >= toRemove->degree())
        {
            throw std::out_of_range("too few children");
        }

        child = toRemove->getChild(toPromote);

        toRemove->key_ = child->key_;
        toRemove->removeChild(child);

        for (int i = 0; i < child->degree(); i++)
        {
            toRemove->addChild(child->getChild(i));
        }

        child->reset();
        delete child;
    }

    return true;
}

template<typename T>
GeneralTree<T>* GeneralTree<T>::search(T key)
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    if (this->getKey() == key)
    {
        return this;
    }

    GeneralTree<T>* result = nullptr, * child;

    for (int i = 0; i < degree() && !result; i++)
    {
        child = getChild(i);

        if (child)
        {
            result = child->search(key);
        }
    }

    return result;
}

template<typename T>
GeneralTree<T>* GeneralTree<T>::lowestCommonAncestor(T key1, T key2)
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    // algorithm from https://www.scaler.com/topics/lca-of-a-binary-tree/

    // if this node's key is one of the provided getKeyAt, it must be the LCA
    if (this->getKey() == key1 || this->getKey() == key2)
    {
        return this;
    }

    GeneralTree<T>* results[2]; // can't be more than 2 children to contain the getKeyAt
    GeneralTree<T>* current = nullptr, * child = nullptr;
    int count = 0;

    for (int i = 0; i < degree() && count != 2; i++)
    {
        child = getChild(i);

        if (child)
        {
            current = child->lowestCommonAncestor(key1, key2);

            if (current)
            {
                results[count++] = current;
            }
        }
    }

    if (count == 2)
    {
        return this;
    }

    if (count == 1)
    {
        return current;
    }

    return nullptr;
}

// Other operations

template<typename T>
void GeneralTree<T>::reset()
{
    children_->clear(true);
    this->size_ = 0;

    if (this->hasParent())
    {
        ((GeneralTree<T>*)this->getParent())->removeChild(this);
    }

    this->parent_ = nullptr;
}

// Tree Traversals

template<typename T>
T* GeneralTree<T>::preOrder()
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    T* array = new T[this->getSize()];
    preOrder(array, 0);

    return array;
}

template<typename T>
T* GeneralTree<T>::postOrder()
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    T* array = new T[this->getSize()];
    postOrder(array, 0);

    return array;
}

template<typename T>
T* GeneralTree<T>::levelOrder()
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    T* array = new T[this->getSize()];
    LQueue<GeneralTree<T>*> queue;
    GeneralTree<T>* current;
    int index = 0;

    // use BFS

    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        current = queue.dequeue();
        array[index++] = current->getKey();

        for (int i = 0; i < current->degree(); i++)
        {
            queue.enqueue(current->getChild(i));
        }
    }

    return array;
}

// Properties of a BinaryTree / Node

template<typename T>
bool GeneralTree<T>::isLeaf()
{
    return !this->isEmpty() && degree() == 0;
}

template<typename T>
int GeneralTree<T>::degree()
{
    return children_->size();
}

// Getters

template<typename T>
int GeneralTree<T>::getHeight()
{
    if (this->isEmpty())
    {
        return -1;
    }

    int maxHeight = -1, currentHeight;

    for (int i = 0; i < degree(); i++)
    {
        GeneralTree<T>* c = getChild(i);
        currentHeight = c->getHeight();

        if (currentHeight > maxHeight)
        {
            maxHeight = currentHeight;
        }
    }

    return maxHeight + 1;
}

template<typename T>
GeneralTree<T>* GeneralTree<T>::getChild(int index)
{
    if (this->isEmpty() || this->isLeaf())
    {
        throw std::out_of_range("too few children");
    }

    return children_->get(index);
}

// PROTECTED

template<typename T>
bool GeneralTree<T>::removeChild(GeneralTree<T>* child)
{
    bool removed = this->children_->remove(child);

    if (removed)
    {
        Tree<T>* current = this;

        while (current)
        {
            current->size_ -= child->getSize();
            current = current->getParent();
        }

        child->parent_ = nullptr;
    }

    return removed;
}

template<typename T>
void GeneralTree<T>::addChild(GeneralTree<T>* child, int index)
{
    if (!child)
    {
        throw std::invalid_argument("cannot add null child");
    }

    if (index == -1)
    {
        children_->add(child);
    }
    else
    {
        children_->add(child, index);
    }

    Tree<T>* current = this;

    while (current)
    {
        current->size_ += child->getSize();
        current = current->getParent();
    }

    child->parent_ = this;
}

template<typename T>
int GeneralTree<T>::indexOf(GeneralTree<T>* child)
{
    for (int i = 0; i < degree(); i++)
    {
        if (getChild(i) == child)
        {
            return i;
        }
    }

    return -1;
}

// PRIVATES

// Traversals Helpers

template<typename T>
int GeneralTree<T>::preOrder(T* array, int index)
{
    array[index++] = this->getKey();

    for (int i = 0; i < degree(); i++)
    {
        index = getChild(i)->preOrder(array, index);
    }

    return index;
}

template<typename T>
int GeneralTree<T>::postOrder(T* array, int index)
{
    for (int i = 0; i < degree(); i++)
    {
        index = getChild(i)->postOrder(array, index);
    }

    array[index] = this->getKey();

    return index + 1;
}
