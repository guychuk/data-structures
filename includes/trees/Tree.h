#pragma once

#define EMPTY_TREE "this tree is empty"
#define NULL_TREE "got a null pointer"

#include <stdexcept>

template<typename T>
class Tree
{
public:
    // Tree Operations

    /**
     * Insert a new key to the tree.
     * @param key
     * @return the newly created node.
     */
    virtual Tree<T>* insert(T key) = 0;

    /**
     * Remove a key from the tree.
     * @param key
     * @return true iff the key was removed.
     */
    virtual bool remove(T key) = 0;

    /**
     * Search a key in the tree.
     * @param key
     * @return the node containing the key, or nullptr if the key is not in the tree.
     */
    virtual Tree<T>* search(T key) = 0;

    /**
     * Find the lowest (deepest) common ancestor (LCA) of two getKeyAt.
     * @param key1
     * @param key2
     * @return the LCA of key1 and key2.
     * If only one key is in the tree, return its node.
     * If both not in the tree - returns nullptr.
     */
    virtual Tree<T>* lowestCommonAncestor(T key1, T key2) = 0;

    // Other operations

    /**
     * Reset the node (size, pointers, etc).
     */
    virtual void reset() = 0;

    // Tree Traversals

    /**
     * Visit the tree in preOrder traversal (root first, then children left to right).
     * @return an array of the getKeyAt in the tree.
     */
    virtual T* preOrder() = 0;

    /**
     * Visit the tree in postOrder traversal (children left to right, then root).
     * @return an array of the getKeyAt in the tree.
     */
    virtual T* postOrder() = 0;

    /**
     * Visit the tree in levelOrder traversal (BFS, higher to lower levels, left to right).
     * @return an array of the getKeyAt in the tree.
     */
    virtual T* levelOrder() = 0;

    // Properties of a BinaryTree / Node

    /**
     * Check if this is a isLeaf node (it has no children).
     * @return true iff this node is a isLeaf.
     */
    virtual bool isLeaf() = 0;

    /**
     * Calculate the degree of this node (the number of children it has).
     * @return this node's degree.
     */
    virtual int degree() = 0;

    /**
     * @return true iff this node has a non-null parent.
     */
    virtual bool hasParent();

    // Getters

    /**
     * @return the node's key.
     * @throws std::logic_error if the tree is empty.
     */
    virtual T getKey();

    /**
     * @return the node's parent.
     */
    virtual Tree<T>* getParent();

    /**
     * Calculate the getHeight of this node (max. distance between this node and a descendant isLeaf).
     * @return this node's getHeight.
     */
    virtual int getHeight() = 0;

    /**
     * Calculate the getSize of this tree (number of getKeyAt in it).
     * @return this tree's getSize.
     */
    virtual int getSize();

    /**
     * Check if the tree is empty.
     * @return true iff the tree is empty.
     */
    virtual bool isEmpty();

    // Friend Classes

    template<typename E> friend class GeneralTree;

protected:
    T key_;
    Tree<T>* parent_;
    int size_;
};

// Properties of a BinaryTree / Node

template<typename T>
bool Tree<T>::hasParent()
{
    return !isEmpty() && parent_ != nullptr;
}

// Getters

template<typename T>
T Tree<T>::getKey()
{
    if (isEmpty())
    {
        throw std::logic_error(EMPTY_TREE);
    }

    return key_;
}

template<typename T>
Tree<T>* Tree<T>::getParent()
{
    if (hasParent())
    {
        return parent_;
    }

    return nullptr;
}

template<typename T>
int Tree<T>::getSize()
{
    return size_;
}

template<typename T>
bool Tree<T>::isEmpty()
{
    return size_ == 0;
}
