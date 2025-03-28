#pragma once

#include "BinaryTree.h"
#include "BST.h"

template<typename T>
class AVLTree : public BST<T>
{
public:
    // Constructors

    AVLTree();

    explicit AVLTree(T key);

    // Tree Operations

    AVLTree<T>* insert(T key);

    AVLTree<T>* search(T key);

    bool remove(T key);

    // Getters

    int getHeight();

    // Types of Binary Trees

    bool isBalancedTree();

private:
    int height_;

    // Rotations

    /**
     * Perform a left rotation on a node.
     * @param x the node to rotate.
     */
    void leftRotation(AVLTree<T>* x);

    /**
     * Perform a right rotation on a node.
     * @param x the node to rotate.
     */
    void rightRotation(AVLTree<T>* x);

    // Updating data

    /**
 * Fix / recalculate the size of this node (assuming the sizes of the descendants are true).
 * @return true iff the size has changed.
 */
    bool fixSize();

    /**
     * Fix / recalculate the height of this node (assuming the height of the descendants are true).
     * @return true iff the height has changed.
     */
    bool fixHeight();

    /**
     * Update the sizes of this tree and all of its ancestors.
     */
    void updateSizesUp();

    /**
     * Update the height of this tree and all of its ancestors.
     */
    void updateHeightsUp();

    // Balancing

    /**
     * Check if the current node is balanced (the height diff. of left and right is at most 1).
     * @return true iff the node is balanced.
     */
    bool isBalancedNode();

    /**
     * Balance a given node.
     * @param x a node to balance.
     */
    void balance(AVLTree<T>* x);

    /**
     * Balance a node and all of its ancestors.
     * @param node the node to balance first.
     */
    void balanceUp(AVLTree<T>* node);

    // Tree Operations

    /**
     * Insert a key to the tree rooted in this node.
     * @param newNode an already created NEW node to insert.
     */
    void insert(AVLTree* newNode);

    /**
     * Remove a key from the tree rooted in this node.
     * @param toRemove the actual node containing the value to remove.
     * @warning this function assumes that toRemove is not a tree with size 1.
     */
    bool removeNode(AVLTree<T>* toRemove);
};

// Constructor

template<typename T>
AVLTree<T>::AVLTree() : BST<T>()
{
    height_ = -1;
}

template<typename T>
AVLTree<T>::AVLTree(T key) : BST<T>(key)
{
    height_ = 0;
}

// Tree Operations

template<typename T>
AVLTree<T>* AVLTree<T>::insert(T key)
{
    // change this node's data, do not make a new node
    if (this->isEmpty())
    {
        this->size_ = 1;
        this->height_ = 0;
        this->key_ = key;
        return this;
    }

    auto* newNode = new AVLTree<T>(key);

    insert(newNode);

    return newNode;
}

template<typename T>
AVLTree<T>* AVLTree<T>::search(T key)
{
    return (AVLTree<T> *) BST<T>::search(key);
}

template<typename T>
bool AVLTree<T>::remove(T key)
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

// Getters

template<typename T>
int AVLTree<T>::getHeight()
{
    return height_;
}

// Type of Binary Trees

template<typename T>
bool AVLTree<T>::isBalancedTree()
{
    return true;
}

// PRIVATES

// Rotations

template<typename T>
void AVLTree<T>::leftRotation(AVLTree<T>* x)
{
    /*
     *   x
     *  / \
     * c   y    =>      y
     *    / \          / \
     *   a   b        x   b
     *               / \
     *              c   a
     */

    AVLTree<T>* c = (AVLTree<T> *) x->getLeft(),
        * y = (AVLTree<T> *) x->getRight(),
        * a = (AVLTree<T> *) y->getLeft(),
        * b = (AVLTree<T> *) y->getRight(),
        * tmp;

    T xKey = x->getKey();

    // swap the getKeyAt of the root and its right child
    x->key_ = y->getKey();
    y->key_ = xKey;

    // and the variable names as well
    tmp = x;
    x = y;
    y = tmp;

    // fix y's children
    y->right_ = b;

    if (b)
    {
        b->parent_ = y;
    }

    y->left_ = x;

    // fix x's children
    x->left_ = c;

    if (c)
    {
        c->parent_ = x;
    }

    x->right_ = a;

    if (a)
    {
        a->parent_ = x;
    }

    // fix the heights and the sizes
    x->updateHeightsUp();
    x->fixSize();
    y->fixSize();
}

template<typename T>
void AVLTree<T>::rightRotation(AVLTree<T>* x)
{
    /*
     *     x
     *    / \
     *   y   c  =>    y
     *  / \          / \
     * a   b        a   x
     *                 / \
     *                b   getChildAt
     */

    AVLTree<T>* y = (AVLTree<T> *) x->getLeft(),
        * c = (AVLTree<T> *) x->getRight(),
        * a = (AVLTree<T> *) y->getLeft(),
        * b = (AVLTree<T> *) y->getRight(),
        * tmp;

    T rootKey = x->key_;

    // swap the getKeyAt of x and y
    x->key_ = y->getKey();
    y->key_ = rootKey;

    // and the variable names as well
    tmp = x;
    x = y;
    y = tmp;

    // fix y's children
    y->left_ = a;

    if (a)
    {
        a->parent_ = y;
    }

    y->right_ = x;  // its parent is already what it should be

    // fix x's children
    x->left_ = b;

    if (b)
    {
        b->parent_ = x;
    }

    x->right_ = c;

    if (c)
    {
        c->parent_ = x;
    }

    // fix the heights and the sizes
    x->updateHeightsUp();
    x->fixSize();
    y->fixSize();
}

// Updating data

template<typename T>
bool AVLTree<T>::fixSize()
{
    int oldSize = this->getSize();
    int leftSize = this->hasLeft() ? this->getLeft()->getSize() : 0;
    int rightSize = this->hasRight() ? this->getRight()->getSize() : 0;
    this->size_ = leftSize + rightSize + 1;
    return oldSize != this->getSize();
}

template<typename T>
bool AVLTree<T>::fixHeight()
{
    int oldHeight = getHeight();
    int leftHeight = this->hasLeft() ? ((AVLTree<T> *) this->getLeft())->getHeight() : -1;
    int rightHeight = this->hasRight() ? ((AVLTree<T> *) this->getRight())->getHeight() : -1;
    this->height_ = leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    return oldHeight != getHeight();
}

template<typename T>
void AVLTree<T>::updateSizesUp()
{
    AVLTree<T>* current = this;

    while (current)
    {
        current->fixSize();
        current = (AVLTree<T> *) current->getParent();
    }
}

template<typename T>
void AVLTree<T>::updateHeightsUp()
{
    AVLTree<T>* current = this;

    while (current)
    {
        current->fixHeight();
        current = (AVLTree<T> *) current->getParent();
    }
}

// Balancing

template<typename T>
bool AVLTree<T>::isBalancedNode()
{
    int leftHeight = this->hasLeft() ? ((AVLTree<T> *) this->getLeft())->getHeight() : -1;
    int rightHeight = this->hasRight() ? ((AVLTree<T> *) this->getRight())->getHeight() : -1;
    return abs(leftHeight - rightHeight) <= 1;
}

template<typename T>
void AVLTree<T>::balance(AVLTree<T>* x)
{
    if (!x || x->isBalancedNode())
    {
        return;
    }

    int leftHeight = x->hasLeft() ?
        ((AVLTree<T> *) x->getLeft())->getHeight() : -1;
    int leftLeftHeight = x->hasLeft() && x->getLeft()->hasLeft() ?
        ((AVLTree<T> *) x->left_->left_)->height_ : -1;
    int leftRightHeight = x->hasLeft() && x->getLeft()->hasRight() ?
        ((AVLTree<T> *) x->left_->right_)->height_ : -1;

    int rightHeight = x->hasRight() ?
        ((AVLTree<T> *) x->getRight())->getHeight() : -1;
    int rightLeftHeight = x->hasRight() && x->getRight()->hasLeft() ?
        ((AVLTree<T> *) x->getRight()->getLeft())->getHeight() : -1;
    int rightRightHeight = x->hasRight() && x->getRight()->hasRight() ?
        ((AVLTree<T> *) x->getRight()->getRight())->getHeight() : -1;

    // Left
    if (leftHeight > rightHeight)
    {
        // Left-Left
        if (leftLeftHeight >= leftRightHeight)
        {
            rightRotation(x);
        }
        // Left-Right
        else
        {
            leftRotation((AVLTree<T> *) x->getLeft());
            rightRotation(x);
        }
    }
    // Right
    else
    {
        // Right-Right
        if (rightLeftHeight <= rightRightHeight)
        {
            leftRotation(x);
        }
        // Right-Left
        else
        {
            rightRotation((AVLTree<T> *) x->getRight());
            leftRotation(x);
        }
    }
}

template<typename T>
void AVLTree<T>::balanceUp(AVLTree<T>* node)
{
    if (!node)
    {
        return;
    }

    AVLTree<T>* x = node;

    do
    {
        while (x && !x->fixHeight() && x->isBalancedNode())
        {
            x = (AVLTree<T> *) x->getParent();
        }

        if (x)
        {
            balance(x);
            x = (AVLTree<T> *) x->getParent();
        }
    } while (x);
}

// Tree Operations

template<typename T>
void AVLTree<T>::insert(AVLTree* newNode)
{
    BST<T>::insert(newNode);
    balanceUp(newNode);
}

template<typename T>
bool AVLTree<T>::removeNode(AVLTree<T>* toRemove)
{
    BinaryTree<T>* existingChild, * parent, * tmp, * successorNode;
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
            existingChild->parent_ = (AVLTree<T>*)parent;
        }

        // update the child in the parent node
        if (parent)
        {
            if (toRemove->isLeftChild())
            {
                parent->left_ = (AVLTree<T>*)existingChild;
            }
            else
            {
                parent->right_ = (AVLTree<T>*)existingChild;
            }

            // set the children null so they won't get destroyed
            toRemove->left_ = nullptr;
            toRemove->right_ = nullptr;
            delete toRemove;

            // update sizes and getHeight

            tmp = parent;

            while (parent)
            {
                parent->size_--;
                parent = parent->getParent();
            }

            balanceUp((AVLTree<T> *) tmp);
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

            balanceUp(toRemove);
        }
    }
    else
    {
        successorNode = BST<T>::successor(toRemove);
        successorKey = successorNode->getKey();
        removeNode((AVLTree<T> *) successorNode);
        toRemove->key_ = successorKey;
    }

    return true;
}
