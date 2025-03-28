#pragma once

// TODO: maybe define a new exception class
#define GOT_NULL_NODE "got a null pointer to a node"

#include "BinaryTree.h"

template<typename T>
class BST : public BinaryTree<T>
{
public:
    // Constructors

    /**
     * Create an empty BST.
     */
    BST();

    /**
     * Create a new node with a key.
     * @param key s key to store in this node.
     */
    explicit BST(T key);

    // Tree Operations

    BST<T>* insert(T key);

    bool remove(T key);

    BST<T>* search(T key);

    /**
     * Find the smallest key in the tree.
     * @return the node with the smallest key in the tree.
     */
    BST<T>* minimum();

    /**
     * Find the greatest key in the tree.
     * @return the node with the greatest key in the tree.
     */
    BST<T>* maximum();

    /**
     * Find the successor of a node.
     * @param node
     * @return the successor node of the given node, or nullptr if there is no such node.
     * @note (Wiki) the successor of a node x in BST is the node with the smallest key greater than x's key.
     * @throws std::invalid_argument if the node is null or empty.
     */
    BST<T>* successor(BST<T>* node);

    /**
     * Find the predecessor of a node.
     * @param node
     * @return the predecessor node of the given node, or nullptr if there is no such node.
     * @note (Wiki) the predecessor of a node x in BST is the node with the largest key smaller than x's key.
     * @throws std::invalid_argument if the node is null or empty.
     */
    BST<T>* predecessor(BST<T>* node);

    template<typename U> friend
        class AVLTree;

protected:
    /**
     * Insert a key to the tree rooted in this node.
     * @param newNode an already created NEW node to insert.
     */
    void insert(BST<T>* newNode);

    /**
     * Remove a key from the tree rooted in this node.
     * @param toRemove the actual node containing the value to remove.
     * @warning this function assumes that toRemove is not a tree with size 1.
     */
    bool removeNode(BST<T>* toRemove);
};

// Constructors

template<typename T>
BST<T>::BST() : BinaryTree<T>()
{

}

template<typename T>
BST<T>::BST(T key) : BinaryTree<T>(key)
{

}

// Tree Operations

template<typename T>
BST<T>* BST<T>::insert(T key)
{
    // change this node's data, do not make a new node
    if (this->isEmpty())
    {
        this->size_ = 1;
        this->key_ = key;
        return this;
    }

    auto* newNode = new BST<T>(key);

    insert(newNode);

    return newNode;
}

template<typename T>
bool BST<T>::remove(T key)
{
    if (this->isEmpty())
    {
        return false;
    }

    if (this->getSize() == 1 && this->getKey() == key)
    {
        this->size_ = 0;
        return true;
    }

    return removeNode(search(key));
}

template<typename T>
BST<T>* BST<T>::search(T key)
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    BinaryTree<T>* current = this;

    while (current && current->getKey() != key)
    {
        if (current->getKey() > key)
        {
            current = current->getLeft();
        }
        else
        {
            current = current->getRight();
        }
    }

    return (BST<T> *) current;
}

template<typename T>
BST<T>* BST<T>::minimum()
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    BinaryTree<T>* minNode = this;

    while (minNode->hasLeft())
    {
        minNode = minNode->getLeft();
    }

    return (BST<T> *) minNode;
}

template<typename T>
BST<T>* BST<T>::maximum()
{
    if (this->isEmpty())
    {
        return nullptr;
    }

    BinaryTree<T>* maxNode = this;

    while (maxNode->hasRight())
    {
        maxNode = maxNode->getRight();
    }

    return (BST<T> *) maxNode;
}

template<typename T>
BST<T>* BST<T>::successor(BST<T>* node)
{
    if (!node || node->isEmpty())
    {
        throw std::invalid_argument(GOT_NULL_NODE);
    }

    if (node->hasRight())
    {
        // smallest that is still greater
        return ((BST<T> *) (node->getRight()))->minimum();
    }

    // find the first ancestor whose left subtree contains the node

    BinaryTree<T>* current = node;

    while (current && current->isRightChild())
    {
        current = current->getParent();
    }

    return (BST<T> *) current->getParent();
}

template<typename T>
BST<T>* BST<T>::predecessor(BST<T>* node)
{
    if (!node || node->isEmpty())
    {
        throw std::invalid_argument(GOT_NULL_NODE);
    }

    if (node->hasLeft())
    {
        // greatest that is still smaller
        ((BST<T> *) (node->getRight()))->minimum();
    }

    // find the first ancestor whose right subtree contains the node

    BinaryTree<T>* current = node;

    while (current && current->isLeftChild())
    {
        current = current->getParent();
    }

    return current->getParent();
}

// PROTECTED

template<typename T>
void BST<T>::insert(BST<T>* newNode)
{
    BinaryTree<T>* parentNode = this;
    bool foundParent = false, insertLeft;

    // search the (future) parent of the new node

    do
    {
        insertLeft = parentNode->getKey() > newNode->getKey();

        // we can insert under the current parent
        if ((insertLeft && !parentNode->hasLeft()) || (!insertLeft && !parentNode->hasRight()))
        {
            foundParent = true;
        }
        else
        {
            parentNode = insertLeft ? parentNode->getLeft() : parentNode->getRight();
        }
    } while (!foundParent);

    // insert the node

    if (insertLeft)
    {
        parentNode->left_ = newNode;
    }
    else
    {
        parentNode->right_ = newNode;
    }

    newNode->parent_ = (BST<T> *) parentNode;

    // update sizes and getHeight

    while (parentNode)
    {
        parentNode->size_++;
        parentNode = parentNode->getParent();
    }
}

// PRIVATES

template<typename T>
bool BST<T>::removeNode(BST<T>* toRemove)
{
    BinaryTree<T>* existingChild, * parent;
    BST<T>* successorNode;
    T successorKey;

    if (!toRemove)
    {
        return false;
    }

    parent = toRemove->getParent();

    if (toRemove->degree() < 2)
    {
        // if toRemove is a isLeaf, the child would be nullptr
        existingChild = toRemove->hasLeft() ? toRemove->getLeft() : toRemove->getRight();

        // update the parent in the child node
        if (existingChild)
        {
            existingChild->parent_ = (BST<T> *) parent;
        }

        // we do not remove the root
        // update the child in the parent node
        if (parent)
        {
            if (toRemove->isLeftChild())
            {
                parent->left_ = (BST<T> *) existingChild;
            }
            else
            {
                parent->right_ = (BST<T> *) existingChild;
            }

            // set the children null so they won't get destroyed
            toRemove->left_ = nullptr;
            toRemove->right_ = nullptr;
            delete toRemove;

            // update sizes and getHeight

            while (parent)
            {
                parent->size_--;
                parent = parent->getParent();
            }
        }
        // removing the root, also it is impossible that existingChild and parent are both nulls,
        // because then toRemove is a isLeaf and a root and this function assumes this is NOT the case
        else
        {
            toRemove->key_ = existingChild->getKey();
            toRemove->left_ = existingChild->getLeft();
            toRemove->right_ = existingChild->getRight();

            if (existingChild->hasLeft())
            {
                existingChild->left_->parent_ = toRemove;
            }

            if (existingChild->hasRight())
            {
                existingChild->right_->parent_ = toRemove;
            }

            existingChild->left_ = nullptr;
            existingChild->right_ = nullptr;
            delete existingChild;

            toRemove->size_--;
        }
    }
    // the node has two children, remove the successor and copy its data to the given node
    else
    {
        successorNode = successor(toRemove);
        successorKey = successorNode->getKey();
        removeNode(successorNode);
        toRemove->key_ = successorKey;
    }

    return true;
}
