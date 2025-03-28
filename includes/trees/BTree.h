#pragma once

#include "BTreeNode.h"

template <typename S>
class BTree
{
public:
    // Constructors

    explicit BTree(int t);

    // Destructor

    ~BTree();

    // Getters

    BTreeNode<S>* root();

    int t();

    // Tree Operations

    BTreeNode<S> *search(int k);

    void insert(int k, S* data);

    bool remove(int k);

    // Tree Properties

    int getSize();

    int getHeight();

private:
    BTreeNode<S>* root_;
    int t_;

    void insertNonFull(BTreeNode<S>* x, int k, S* data);

    bool remove(BTreeNode<S>* x, int k);
};

// Constructor

template<typename S>
BTree<S>::BTree(int t)
{
    root_ = new BTreeNode<S>(t);
    t_ = t;
}

// Destructor

template<typename S>
BTree<S>::~BTree()
{
    delete root_;
    root_ = nullptr;
}

// Getters

template<typename S>
BTreeNode<S>* BTree<S>::root()
{
    return root_;
}

template<typename S>
int BTree<S>::t()
{
    return t_;
}

// Tree Operations

template<typename S>
BTreeNode<S>* BTree<S>::search(int k)
{
    return root_ ? root_->search(k) : nullptr;
}

template<typename S>
void BTree<S>::insert(int k, S* data)
{
    BTreeNode<S>* oldRoot = root(), * newRoot = nullptr;

    // if the root is full, split it and create a new root
    if (oldRoot->isFull())
    {
        newRoot = new BTreeNode<S>(t());
        newRoot->addChild(oldRoot);
        newRoot->splitChild(0);
        root_ = newRoot;
    }
    
    insertNonFull(root_, k, data);
}

template<typename S>
bool BTree<S>::remove(int k)
{
    return remove(root(), k);
}

template<typename S>
inline int BTree<S>::getSize()
{
    return root()->getSize();
}

template<typename S>
inline int BTree<S>::getHeight()
{
    if (root()->getNumOfKeys() == 0)
    {
        return -1;
    }

    int height = 0;
    BTreeNode<S> *current = root();

    while (!current->isLeaf())
    {
        height++;
        current = current->getChildAt(0);
    }

    return height;
}

template<typename S>
void BTree<S>::insertNonFull(BTreeNode<S>* x, int k, S* data)
{
    int i = 0;

    // find the first key that is greater then k

    while (i < x->getNumOfKeys() && x->getKeyAt(i) < k)
    {
        i++;
    }

    if (x->isLeaf())
    {
        // add the key in the correct place/spot in this node
        x->addKeyAndData(k, data, i);
    }
    else
    {
        if (x->getChildAt(i)->isFull())
        {
            x->splitChild(i);

            // k is greater than the new key (the median in the next child)
            if (k > x->getKeyAt(i))
            {
                i++;
            }
        }

        insertNonFull(x->getChildAt(i), k, data);
    }
}

template<typename S>
bool BTree<S>::remove(BTreeNode<S>* x, int k)
{
    bool removed = false;

    BTreeNode<S>* successor = nullptr;

    int i = 0;

    int b;

    // find the first key that is greater/equal to k

    while (i < x->getNumOfKeys() && x->getKeyAt(i) < k)
    {
        i++;
    }

    if (x->isLeaf())
    {
        if (i < x->getNumOfKeys() && x->getKeyAt(i) == k)
        {
            x->removeKeyAndData(i);
            removed = true;
        }
    }
    else
    {
        if (i < x->getNumOfKeys() && x->getKeyAt(i) == k)
        {
            // if the key is in this internal node, remove its successorand swap the key and the data
            
            successor = x->getChildAt(i);

            while (!successor->isLeaf())
            {
                successor = successor->getChildAt(0);
            }

            x->keys_[i] = successor->getKeyAt(0);
            x->data_[i] = successor->getDataAt(0);
            
            removed = remove(successor, successor->getKeyAt(0));
        }
        else
        {
            if (x->getChildAt(i)->getNumOfKeys() == t() - 1)
            {
                // if the next child is with minimal number of children,
                // try shifting and if this is impossible - do merging

                if (!x->shift(i))
                {
                    x->mergeChildren(i);
                }

                if (k < x->getKeyAt(i - 1))
                {
                    i--;
                }
            }

            removed = remove(x->getChildAt(i), k);
        }
    }

    return removed;
}
