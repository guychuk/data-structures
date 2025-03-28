#pragma once

#include "GeneralTree.h"

template<typename T>
class NAryTree : public GeneralTree<T>
{
public:
    // Constructors

    /**
     * Create a new N-Ary tree.
     * @param N the maximum number of children per node.
     */
    explicit NAryTree(int N);

    /**
     * Create a new N-Ary tree and set its root key.
     * @param N the maximum number of children per node.
     */
    explicit NAryTree(int N, T key);

    // Tree Operations

    virtual NAryTree<T>* insert(T key);

    virtual NAryTree<T>* insert(T key, T parent);

    /**
     * Insert a new key to the tree.
     * @param key
     * @param parent the parent node to the new key.
     * @return the newly created node.
     */
    virtual NAryTree<T>* insert(T key, NAryTree<T>* parent);

    /**
     * Insert a node to the tree.
     * @param newNode
     * @param parent the key of the parent node to the new key.
     */
    virtual NAryTree<T>* insert(NAryTree<T>* newNode, NAryTree<T>* parent);

    virtual NAryTree<T>* search(T key);

    virtual NAryTree<T>* lowestCommonAncestor(T key1, T key2);

    // Getters

    /**
     * @return the maximum number of children to this node.
     */
    virtual int maxDegree();

    NAryTree<T>* getChild(int index);

    // Types of Trees

    /**
     * Check if the tree is a full getNumOfKeys-ary tree.
     * @return true iff the tree is a full tree.
     * @note A full n-ary tree is an n-ary tree where within each level every node has 0 or getNumOfKeys children.
     */
    bool isFullTree();

    /**
     * Check if the tree is a complete getNumOfKeys-ary tree.
     * @return true iff the tree is a complete tree.
     * @note A complete getNumOfKeys-ary tree is a tree in which all levels are completely filled,
     * except possibly for the last level. In the last level,
     * all nodes are positioned as left as possible.
     */
    bool isCompleteTree();

    /**
     * Check if the tree is a perfect getNumOfKeys-ary tree.
     * @return true iff the tree is a perfect tree.
     * @note A perfect n-ary tree is a tree in which all internal nodes have exactly "getNumOfKeys" children,
     * and all leaves are at the same level.
     */
    bool isPerfectTree();

protected:
    int N_;

    // Types of trees Helpers

    /**
     * A helper method of isPerfectTree(void).
     * @param nodeDepth the current node's depth in the tree rooted in the caller node.
     * @param treeHeight the getHeight of the caller node.
     * @return true iff the tree rooted in this node is a perfect getNumOfKeys-ary tree.
     */
    bool isPerfectHelper(int nodeDepth, int treeHeight);
};

// Constructors

template<typename T>
NAryTree<T>::NAryTree(int N) : GeneralTree<T>()
{
    N_ = N;
}

template<typename T>
NAryTree<T>::NAryTree(int N, T key) : GeneralTree<T>(key)
{
    N_ = N;
}

// Tree Operations

template<typename T>
NAryTree<T>* NAryTree<T>::insert(T key)
{
    return insert(key, this);
}

template<typename T>
NAryTree<T>* NAryTree<T>::insert(T key, T parent)
{
    return insert(key, (NAryTree<T> *) (this->search(parent)));
}

template<typename T>
NAryTree<T>* NAryTree<T>::insert(T key, NAryTree<T>* parent)
{
    if (parent->degree() == parent->maxDegree())
    {
        throw std::logic_error("the parent is full, cannot insert a new child");
    }

    return (NAryTree<T> *) (GeneralTree<T>::insert(new NAryTree<T>(maxDegree(), key), parent));
}

template<typename T>
NAryTree<T>* NAryTree<T>::insert(NAryTree<T>* newNode, NAryTree<T>* parent)
{
    if (parent->degree() == parent->maxDegree())
    {
        throw std::logic_error("the parent is full, cannot insert a new child");
    }

    return (NAryTree<T> *) (GeneralTree<T>::insert(newNode, parent));
}

template<typename T>
NAryTree<T>* NAryTree<T>::search(T key)
{
    return (NAryTree<T> *) (GeneralTree<T>::search(key));
}

template<typename T>
NAryTree<T>* NAryTree<T>::lowestCommonAncestor(T key1, T key2)
{
    return (NAryTree<T> *) (GeneralTree<T>::lowestCommonAncestor(key1, key2));
}

// Getters

template<typename T>
int NAryTree<T>::maxDegree()
{
    return N_;
}

template<typename T>
NAryTree<T>* NAryTree<T>::getChild(int index)
{
    return (NAryTree<T> *) (GeneralTree<T>::getChild(index));
}

// Types of Trees

template<typename T>
bool NAryTree<T>::isFullTree()
{
    if (this->isEmpty())
    {
        throw std::logic_error(EMPTY_TREE);
    }

    if (this->isLeaf())
    {
        return true;
    }

    bool thisIsFull = this->degree() == maxDegree();

    for (int i = 0; i < this->degree() && thisIsFull; i++)
    {
        thisIsFull = this->getChild(i)->isFullTree();
    }

    return thisIsFull;
}

template<typename T>
bool NAryTree<T>::isCompleteTree()
{
    // use the BFS algorithm
    // idea taken from YouTube: https://youtu.be/olbiZ-EOSig

    if (this->isEmpty())
    {
        throw std::logic_error(EMPTY_TREE);
    }

    if (this->isLeaf())
    {
        return true;
    }

    LQueue<NAryTree<T>*> queue;
    NAryTree<T>* current;
    bool expectNulls = false;
    int currentDegree;

    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        current = queue.dequeue();

        if (current == nullptr)
        {
            expectNulls = true;
        }
        else
        {
            if (expectNulls)
            {
                return false;
            }

            currentDegree = current->degree();

            for (int i = 0; i < currentDegree; i++)
            {
                queue.enqueue(current->getChild(i));
            }

            for (int i = currentDegree; i < maxDegree(); i++)
            {
                queue.enqueue(nullptr);
            }
        }
    }

    return true;
}

template<typename T>
bool NAryTree<T>::isPerfectHelper(int nodeDepth, int treeHeight)
{
    if (this->isEmpty())
    {
        throw std::logic_error(EMPTY_TREE);
    }

    if (this->isLeaf())
    {
        return nodeDepth == treeHeight;
    }

    for (int i = 0; i < this->degree(); i++)
    {
        if (!getChild(i)->isPerfectHelper(nodeDepth + 1, treeHeight))
        {
            return false;
        }
    }

    return true;
}

// PRIVATES

// Types of trees Helpers

template<typename T>
bool NAryTree<T>::isPerfectTree()
{
    return isPerfectHelper(0, this->getHeight());
}
