#pragma once

#define EMPTY_TREE "this tree is empty"

#include "../queues/LQueue.h"
#include "Tree.h"

template<typename T>
class BinaryTree : public Tree<T>
{
public:
    // Constructors

    /**
     * Create an empty binary tree.
     */
    BinaryTree();

    /**
     * Create a new node with a key.
     * @param key s key to store in this node.
     */
    explicit BinaryTree(T key);

    // Destructor

    /**
     * Destructor.
     * Deletes and resets this node and all of its descendants.
     */
    virtual ~BinaryTree();

    // Tree Operations

    /**
     * Insert a key to the tree rooted in this node.
     * @param key
     * @return the new node.
     */
    virtual BinaryTree<T>* insert(T key);

    /**
     * Remove a key from the tree rooted in this node.
     * @param key
     * @note if the key appears more than once, the method removes the
     * first occurrence according to a preOrder traversal.
     */
    virtual bool remove(T key);

    /**
     * Search a key in the tree rooted in this node (using preOrder traversal).
     * @param key
     * @return a pointer to a descendant with this key, or nullptr if there is no such node.
     */
    virtual BinaryTree<T>* search(T key);

    /**
     * Find the lowest (deepest) common ancestor (LCA) of two getKeyAt.
     * @param key1
     * @param key2
     * @return the LCA of key1 and key2.
     * If only one key is in the tree, return its node.
     * If both not in the tree - returns nullptr.
     */
    BinaryTree<T>* lowestCommonAncestor(T key1, T key2);

    // Other operations

    /**
     * Reset the node (size, pointers, etc).
     */
    void reset();

    // Tree Traversals

    /**
     * Visit the tree in inOrder traversal (left, root, right).
     * @return an array of the getKeyAt in the tree.
     */
    T* inOrder();

    /**
     * Visit the tree in preOrder traversal (root, left, right).
     * @return an array of the getKeyAt in the tree.
     */
    T* preOrder();

    /**
     * Visit the tree in postOrder traversal (left, right, root).
     * @return an array of the getKeyAt in the tree.
     */
    T* postOrder();

    /**
     * Visit the tree in levelOrder traversal (BFS, higher to lower levels, left to right).
     * @return an array of the getKeyAt in the tree.
     */
    T* levelOrder();

    // Types of Binary Trees

    /**
     * Check if the tree is balanced (in all nodes, the getHeight difference between left and right is at most 1).
     * @return true iff the tree is balanced.
     */
    virtual bool isBalancedTree();

    /**
     * Check if the tree is a complete binary tree.
     * @note (Wiki) A complete binary tree is a binary tree in which every level,
     * except possibly the last, is completely filled,
     * and all nodes in the last level are as far left as possible.
     * @return true iff the tree is a complete tree.
     */
    bool isCompleteTree();

    /**
     * Check if the tree is a full binary tree.
     * @note (Wiki) A full binary tree is a tree in which every node has either 0 or 2 children.
     * @return true iff the tree is a complete tree.
     */
    bool isFullTree();

    /**
     * Check if the tree is a prefect binary tree.
     * @note (Wiki) A perfect binary tree is a binary tree in which all interior nodes have two children
     * and all leaves have the same depth or same level.
     * @return true iff the tree is a complete tree.
     */
    bool isPerfectTree();

    // Properties of a BinaryTree / Node

    /**
     * Check if this is a isLeaf node (it has no children).
     * @return true iff this node is a isLeaf.
     */
    bool isLeaf();

    /**
     * Calculate the degree of this node (the number of children it has).
     * @return this node's degree.
     */
    int degree();

    /**
     * @return true iff this node has a non-null left child.
     */
    bool hasLeft();

    /**
     * @return true iff this node has a non-null right child.
     */
    bool hasRight();

    /**
     * @return true iff this node has a non-null parent.
     */
    bool hasParent();

    /**
     * @return true iff this node is a left child of its parent node.
     */
    bool isLeftChild();

    /**
     * @return true iff this node is a right child of its parent node.
     */
    bool isRightChild();

    // Getters

    /**
     * @return the node's key.
     * @throws std::logic_error if the tree is empty.
     */
    T getKey();

    /**
     * @return the node's left child.
     */
    BinaryTree<T>* getLeft();

    /**
     * @return the node's right child.
     */
    BinaryTree<T>* getRight();

    /**
     * @return the node's parent.
     */
    BinaryTree<T>* getParent();

    /**
     * Calculate the getHeight of this node (max. distance between this node and a descendant isLeaf).
     * @return this node's getHeight.
     */
    virtual int getHeight();

    /**
     * Calculate the getSize of this tree (number of getKeyAt in it).
     * @return this tree's getSize.
     */
    int getSize();

    /**
     * Check if the tree is empty.
     * @return true iff the tree is empty.
     */
    bool isEmpty();

    // Friend Classes

    template<typename E> friend
        class BST;

    template<typename U> friend
        class AVLTree;

protected:
    T key_;
    BinaryTree<T>* left_, * right_, * parent_;
    int size_;

    /**
     * Insert a key to the tree rooted in this node.
     * @param newNode an already created NEW node to insert.
     */
    void insert(BinaryTree<T>* newNode);

private:
    // Traversals Helpers

    /**
     * A helper method of inOrder(void).
     * This method does the actual traversal and inserts the getKeyAt to the array.
     * @param array an array to store the elements.
     * @param index the first available index in the array.
     * @return the first available index in the array after the current call.
     */
    int inOrder(T* array, int index);

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

    // Types of trees Helpers

    /**
     * A helper method of isPerfectTree(void).
     * @param nodeDepth the current node's depth in the tree rooted in the caller node.
     * @param treeHeight the getHeight of the caller node.
     * @return true iff the tree rooted in this node is a perfect binary tree.
     */
    bool isPerfectHelper(int nodeDepth, int treeHeight);
};

// Constructor

template<typename T>
BinaryTree<T>::BinaryTree()
{
    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;
    size_ = 0;
}

template<typename T>
BinaryTree<T>::BinaryTree(T key)
{
    key_ = key;
    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;
    size_ = 1;
}

// Destructor

template<typename T>
BinaryTree<T>::~BinaryTree()
{
    delete left_;
    delete right_;

    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;

    size_ = 0;
}

// Tree Operations

template<typename T>
BinaryTree<T>* BinaryTree<T>::insert(T key)
{
    // this tree is empty, add its first node
    if (size_ == 0)
    {
        size_ = 1;
        key_ = key;
        return this;
    }

    auto* newNode = new BinaryTree<T>(key);

    insert(newNode);

    return newNode;
}

template<typename T>
bool BinaryTree<T>::remove(T key)
{
    BinaryTree<T>* toRemove = search(key), * deepRight = nullptr, * ancestor = nullptr;

    LQueue<BinaryTree<T>*> queue;

    bool deepRightIsLeftChild;

    if (getSize() == 0 || !toRemove)
    {
        return false;
    }

    if (getSize() == 1 && getKey() == key)
    {
        size_ = 0;
        return true;
    }

    // use BFS to find the deepest rightmost node in the tree

    queue.enqueue(this);
    deepRightIsLeftChild = true;

    while (!queue.isEmpty())
    {
        deepRight = queue.dequeue();
        deepRightIsLeftChild = !deepRightIsLeftChild;

        // don't enqueue null pointers

        if (deepRight->hasLeft())
        {
            queue.enqueue(deepRight->getLeft());
        }

        if (deepRight->hasRight())
        {
            queue.enqueue(deepRight->getRight());
        }
    }

    // copy the key of this node to the node to remove,
    // then remove the deepest rightmost node from the tree

    toRemove->key_ = deepRight->getKey();

    ancestor = deepRight->getParent();

    if (deepRightIsLeftChild)
    {
        ancestor->left_ = nullptr;
    }
    else
    {
        ancestor->right_ = nullptr;
    }

    // update the sizes

    while (ancestor)
    {
        ancestor->size_--;
        ancestor = ancestor->getParent();
    }

    delete deepRight;

    return true;
}

template<typename T>
BinaryTree<T>* BinaryTree<T>::search(T key)
{
    if (key == getKey())
    {
        return this;
    }

    BinaryTree<T>* leftSearch = hasLeft() ? getLeft()->search(key) : nullptr;
    BinaryTree<T>* rightSearch = hasRight() ? getRight()->search(key) : nullptr;

    if (leftSearch)
    {
        return leftSearch;
    }

    if (rightSearch)
    {
        return rightSearch;
    }

    return nullptr;
}

template<typename T>
BinaryTree<T>* BinaryTree<T>::lowestCommonAncestor(T key1, T key2)
{
    // algorithm from https://www.scaler.com/topics/lca-of-a-binary-tree/

    // if this node's key is one of the provided getKeyAt, it must be the LCA
    if (getKey() == key1 || getKey() == key2)
    {
        return this;
    }

    // search the getKeyAt in the left and right subtrees
    BinaryTree<T>* foundInLeft = hasLeft() ? getLeft()->lowestCommonAncestor(key1, key2) : nullptr;
    BinaryTree<T>* foundInRight = hasRight() ? getRight()->lowestCommonAncestor(key1, key2) : nullptr;

    // one value in the left subtree and the other in the right subtree
    if (foundInLeft && foundInRight)
    {
        return this;
    }

    // both getKeyAt are in the same subtree
    return foundInLeft ? foundInLeft : foundInRight;
}

// Other Operations

template<typename T>
void BinaryTree<T>::reset()
{
    delete left_;
    delete right_;

    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;

    size_ = 0;
}

// Tree Traversals

template<typename T>
T* BinaryTree<T>::inOrder()
{
    T* array = new T[getSize()];
    inOrder(array, 0);
    return array;
}

template<typename T>
T* BinaryTree<T>::preOrder()
{
    T* array = new T[getSize()];
    preOrder(array, 0);
    return array;
}

template<typename T>
T* BinaryTree<T>::postOrder()
{
    T* array = new T[getSize()];
    postOrder(array, 0);
    return array;
}

template<typename T>
T* BinaryTree<T>::levelOrder()
{
    T* array = new T[getSize()];
    int index = 0;
    LQueue<BinaryTree<T>*> queue;
    BinaryTree<T>* current;

    // use BFS

    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        current = queue.dequeue();

        if (current)
        {
            array[index++] = current->getKey();

            if (current->hasLeft())
            {
                queue.enqueue(current->getLeft());
            }

            if (current->hasRight())
            {
                queue.enqueue(current->getRight());
            }
        }
    }

    return array;
}

// Types of Binary Trees

template<typename T>
bool BinaryTree<T>::isBalancedTree()
{
    int leftHeight = hasLeft() ? getLeft()->getHeight() : -1;
    int rightHeight = hasRight() ? getRight()->getHeight() : -1;

    return abs(leftHeight - rightHeight) <= 1                 // getHeight diff <= 1 (abs. value)
        && (!hasLeft() || getLeft()->isBalancedTree())       // left is balanced
        && (!hasRight() || getRight()->isBalancedTree());    // right is balanced
}

template<typename T>
bool BinaryTree<T>::isCompleteTree()
{
    // use the BFS algorithm
    // idea taken from YouTube: https://youtu.be/olbiZ-EOSig

    LQueue<BinaryTree<T>*> queue;
    BinaryTree<T>* node;
    bool expectNulls = false;

    if (isLeaf())
    {
        return true;
    }

    queue.enqueue(getLeft());
    queue.enqueue(getRight());

    while (!queue.isEmpty())
    {
        node = queue.dequeue();

        if (node != nullptr)
        {
            if (expectNulls)
            {
                return false;
            }

            queue.enqueue(node->getLeft());
            queue.enqueue(node->getRight());
        }
        else
        {
            // the moment we get to a null, we expect all nulls from here
            expectNulls = true;
        }
    }

    return true;
}

template<typename T>
bool BinaryTree<T>::isFullTree()
{
    // the second part will result in false if exactly one of the children
    // doesn't exist, or at least one of them is not full
    return isLeaf() || ((hasLeft() && getLeft()->isFullTree()) && (hasRight() && getRight()->isFullTree()));
}

template<typename T>
bool BinaryTree<T>::isPerfectTree()
{
    return isLeaf() || isPerfectHelper(0, getHeight());
}

// Properties of a BinaryTree / Node

template<typename T>
bool BinaryTree<T>::isLeaf()
{
    return size_ && !left_ && !right_;
}

template<typename T>
int BinaryTree<T>::degree()
{
    return (getLeft() != nullptr) + (getRight() != nullptr);
}

template<typename T>
bool BinaryTree<T>::hasLeft()
{
    return !isEmpty() && left_ != nullptr;
}

template<typename T>
bool BinaryTree<T>::hasRight()
{
    return !isEmpty() && right_ != nullptr;
}

template<typename T>
bool BinaryTree<T>::hasParent()
{
    return !isEmpty() && parent_ != nullptr;
}

template<typename T>
bool BinaryTree<T>::isLeftChild()
{
    return !isEmpty() && hasParent() && getParent()->getLeft() == this;
}

template<typename T>
bool BinaryTree<T>::isRightChild()
{
    return !isEmpty() && hasParent() && getParent()->getRight() == this;
}

// Getters

template<typename T>
T BinaryTree<T>::getKey()
{
    if (isEmpty())
    {
        throw std::logic_error(EMPTY_TREE);
    }

    return key_;
}

template<typename T>
BinaryTree<T>* BinaryTree<T>::getLeft()
{
    return isEmpty() ? nullptr : left_;
}

template<typename T>
BinaryTree<T>* BinaryTree<T>::getRight()
{
    return isEmpty() ? nullptr : right_;
}

template<typename T>
BinaryTree<T>* BinaryTree<T>::getParent()
{
    return isEmpty() ? nullptr : parent_;
}

template<typename T>
int BinaryTree<T>::getHeight()
{
    int leftHeight = hasLeft() ? getLeft()->getHeight() : -1;
    int rightHeight = hasRight() ? getRight()->getHeight() : -1;

    return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
}

template<typename T>
int BinaryTree<T>::getSize()
{
    return size_;
}

template<typename T>
bool BinaryTree<T>::isEmpty()
{
    return size_ == 0;
}

// PROTECTED

template<typename T>
void BinaryTree<T>::insert(BinaryTree<T>* newNode)
{
    LQueue<BinaryTree<T>*> queue;
    BinaryTree<T>* ancestor = nullptr;
    bool inserted = false;

    // use BFS to find the first available place in the tree

    queue.enqueue(this);

    while (!inserted)
    {
        ancestor = queue.dequeue();

        if (!ancestor->hasLeft())
        {
            ancestor->left_ = newNode;
            newNode->parent_ = ancestor;
            inserted = true;
        }
        else if (!ancestor->hasRight())
        {
            ancestor->right_ = newNode;
            newNode->parent_ = ancestor;
            inserted = true;
        }
        else
        {
            queue.enqueue(ancestor->getLeft());
            queue.enqueue(ancestor->getRight());
        }
    }

    // update the sizes of all the trees rooted at the new node's ancestors

    while (ancestor)
    {
        ancestor->size_++;
        ancestor = ancestor->getParent();
    }
}

// PRIVATES

// Tree Traversals Helpers

template<typename T>
int BinaryTree<T>::inOrder(T* array, int index)
{
    if (hasLeft())
    {
        index = getLeft()->inOrder(array, index);
    }

    array[index++] = getKey();

    if (hasRight())
    {
        index = getRight()->inOrder(array, index);
    }

    return index;
}

template<typename T>
int BinaryTree<T>::preOrder(T* array, int index)
{
    array[index++] = getKey();

    if (hasLeft())
    {
        index = getLeft()->preOrder(array, index);
    }

    if (hasRight())
    {
        index = getRight()->preOrder(array, index);
    }

    return index;
}

template<typename T>
int BinaryTree<T>::postOrder(T* array, int index)
{
    if (hasLeft())
    {
        index = getLeft()->postOrder(array, index);
    }

    if (hasRight())
    {
        index = getRight()->postOrder(array, index);
    }

    array[index++] = getKey();

    return index;
}

// Type of trees Helpers

template<typename T>
bool BinaryTree<T>::isPerfectHelper(int nodeDepth, int treeHeight)
{
    if (isLeaf())
    {
        return nodeDepth == treeHeight;
    }

    return (hasLeft() && getLeft()->isPerfectHelper(nodeDepth + 1, treeHeight)) &&
        (hasRight() && getRight()->isPerfectHelper(nodeDepth + 1, treeHeight));
}

