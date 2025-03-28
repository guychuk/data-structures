#pragma once

#include <stdexcept>

/**
 * A B-Tree.
 * @tparam S the type of the satellite data.
 */
template<typename S>
class BTreeNode
{
public:
	// Constructor

	/**
	 * Create an empty BTreeNode.
	 * @param t min. num of keys is t - 1 (except the root) and max. num of getKeyAt is 2t - 1.
	 */
	explicit BTreeNode(int t);

	// Destructor

	~BTreeNode();

	// Getters

	int getNumOfKeys();

	int degree();

	int getKeyAt(int index);

	S *getDataAt(int index);

	BTreeNode<S> *getChildAt(int index);

	bool isLeaf();

	bool isFull();

	int t();

	// Tree Operations

	BTreeNode<S>* search(int k);

	// Tree Properties

	int getSize();

	template<typename T> friend
		class BTree;

private:
	int* keys_;
	S** data_;
	BTreeNode<S>** children_;
	int t_;

	int numOfKeys_;
	int degree_;

	// Helper functions

	/**
	 * Split a child of this node.
	 * Move the median keys if the child to this node, and divide the node to left and right.
	 * @param i the index of the child to split.
	 */
	void splitChild(int i);

	/**
	 * Merge a child with its sibling.
	 * @param i the index of the child to merge.
	 */
	void mergeChildren(int i);

	/**
	 * Shift a key from one node to another (child -> parent -> sibling).
	 * @param i the index of the child to take a key from.
	 * @return true iff a the method shifted a key.
	 */
	bool shift(int i);

	bool addKeyAndData(int key, S* data, int i = -1);

	bool addChild(BTreeNode<S>* child, int i = -1);

	bool removeKeyAndData(int i);

	bool removeChild(int i);
};

// Constructor

template<typename S>
BTreeNode<S>::BTreeNode(int t)
{
	keys_ = new int[2 * t - 1];
	data_ = new S * [2 * t - 1];
	children_ = new BTreeNode<S> * [2 * t];
	t_ = t;

	numOfKeys_ = 0;
	degree_ = 0;
}

// Destructor

template<typename S>
BTreeNode<S>::~BTreeNode()
{
	for (int i = 0; i < degree_; i++)
	{
		delete children_[i];
	}

	degree_ = 0;

	delete[] keys_;
	delete[] data_;
	delete[] children_;
}

// Getters

template<typename S>
int BTreeNode<S>::getNumOfKeys()
{
	return numOfKeys_;
}

template<typename S>
int BTreeNode<S>::degree()
{
	return degree_;
}

template<typename S>
int BTreeNode<S>::getKeyAt(int i)
{
	if (getNumOfKeys() <= i)
	{
		throw std::out_of_range("index of key out of range");
	}

	return keys_[i];
}

template<typename S>
S* BTreeNode<S>::getDataAt(int i)
{
	if (getNumOfKeys() <= i)
	{
		throw std::out_of_range("index of data out of range");
	}

	return data_[i];
}

template<typename S>
BTreeNode<S>* BTreeNode<S>::getChildAt(int i)
{
	if (degree() <= i)
	{
		throw std::out_of_range("index of child out of range");
	}

	return children_[i];
}

template<typename S>
bool BTreeNode<S>::isLeaf()
{
	return degree() == 0;
}

template<typename S>
bool BTreeNode<S>::isFull()
{
	return getNumOfKeys() == 2 * t() - 1;
}

template<typename S>
int BTreeNode<S>::t()
{
	return t_;
}

// Tree Operations

template<typename S>
BTreeNode<S>* BTreeNode<S>::search(int k)
{
	int i = 0;

	// find the index of the first key that is greater/equals k
	while (i < getNumOfKeys() && k > getKeyAt(i))
	{
		i++;
	}

	// found it here
	if (i < getNumOfKeys() && k == getKeyAt(i))
	{
		return this;
	}

	if (i < degree())
	{
		return getChildAt(i)->search(k);
	}

	return nullptr;
}

template<typename S>
int BTreeNode<S>::getSize()
{
	int size = getNumOfKeys();

	for (int i = 0; i < degree(); i++)
	{
		size += getChildAt(i)->getSize();
	}

	return size;
}

template<typename S>
void BTreeNode<S>::splitChild(int i)
{
	BTreeNode<S>* childToSplit = getChildAt(i);
	BTreeNode<S>* newNode = new BTreeNode<S>(t());	// this node will contain the upper half of keys, data and children of the child to split
	
	// copy the keys and the data to the new child

	for (int j = t(); j < 2 * t() - 1; j++)
	{
		newNode->addKeyAndData(childToSplit->getKeyAt(j), childToSplit->getDataAt(j));
	}

	// copy the children

	for (int j = t(); j < childToSplit->degree(); j++)
	{
		newNode->addChild(childToSplit->getChildAt(j));
	}

	// add the new node to the right of the child
	addChild(newNode, i + 1);

	// add the median key and data in the child to this node
	addKeyAndData(childToSplit->getKeyAt(t() - 1), childToSplit->getDataAt(t() - 1), i);

	// delete from the child
	childToSplit->numOfKeys_ = t() - 1;
	childToSplit->degree_ = std::min(t(), childToSplit->degree());
}

template<typename S>
void BTreeNode<S>::mergeChildren(int i)
{
	BTreeNode<S>* mergeTo = nullptr, * mergeFrom = nullptr;
	int indexOfSeparator = 0;

	if (0 < i)
	{
		// the child has a left sibling

		indexOfSeparator = i - 1;
		mergeFrom = getChildAt(i);
		mergeTo = getChildAt(i - 1);	// the left sibling
	}
	else /*if (i < degree() - 1)*/ // it is impossible for the child to be without siblings and not a leaf or a root
	{
		// the child has a right sibling
		indexOfSeparator = i;
		mergeFrom = getChildAt(i + 1);
		mergeTo = getChildAt(i);
	}

	// move the seperator key (and data) to the end of the child that gets new values

	mergeTo->addKeyAndData(getKeyAt(indexOfSeparator), getDataAt(indexOfSeparator));
	removeKeyAndData(indexOfSeparator);

	// move the key, data and children between the children

	for (int j = 0; j < mergeFrom->getNumOfKeys(); j++)
	{
		mergeTo->addKeyAndData(mergeFrom->getKeyAt(j), mergeFrom->getDataAt(j));
	}

	for (int j = 0; j < mergeFrom->degree(); j++)
	{
		mergeTo->addChild(mergeFrom->getChildAt(j));
	}

	mergeFrom->degree_ = 0;
	removeChild(indexOfSeparator + 1);
	delete mergeFrom;
}

template<typename S>
bool BTreeNode<S>::shift(int i)
{
	BTreeNode<S>* child = getChildAt(i);
	BTreeNode<S>* leftSibling = 0 < i ? getChildAt(i - 1) : nullptr;
	BTreeNode<S>* rightSibling = i < degree() - 1 ? getChildAt(i + 1) : nullptr;
	bool shifted = false;

	if (leftSibling && leftSibling->getNumOfKeys() >= t())
	{
		// if the child has a left sibling and we can take one key from it, perform shifting

		// add to the child the key and data that seperates the children
		child->addKeyAndData(getKeyAt(i - 1), getDataAt(i - 1), 0);

		// replace this key with the last key and data in the left sibling
		keys_[i - 1] = leftSibling->getKeyAt(leftSibling->getNumOfKeys() - 1);
		data_[i - 1] = leftSibling->getDataAt(leftSibling->getNumOfKeys() - 1);
		leftSibling->numOfKeys_--;

		// move the last child of the left sibling to be the first child of the child
		if (leftSibling->degree() == leftSibling->getNumOfKeys() + 1)
		{
			child->addChild(leftSibling->getChildAt(leftSibling->degree()), 0);
			leftSibling->degree_--;
		}

		shifted = true;
	}
	else if (rightSibling && rightSibling->getNumOfKeys() >= t())
	{
		// if the child has a right sibling and we can take one key from it, perform shifting

		// add to the child the key and data that seperates the children
		child->addKeyAndData(getKeyAt(i), getDataAt(i));

		// replace this key with the first key and data in the right sibling
		keys_[i] = rightSibling->getKeyAt(0);
		data_[i] = rightSibling->getDataAt(0);
		rightSibling->removeKeyAndData(0);

		// move the first child of the right sibling to be the last child of the child
		if (!rightSibling->isLeaf())
		{
			child->addChild(rightSibling->getChildAt(0));
			rightSibling->removeChild(0);
		}

		shifted = true;
	}
	
	return shifted;
}

template<typename S>
bool BTreeNode<S>::addKeyAndData(int key, S* data, int i)
{
	if (isFull())
	{
		return false;
	}

	if (i == -1)
	{
		keys_[getNumOfKeys()] = key;
		data_[getNumOfKeys()] = data;
	}
	else
	{
		for (int j = getNumOfKeys(); j > i; j--)
		{
			keys_[j] = keys_[j - 1];
			data_[j] = data_[j - 1];
		}

		keys_[i] = key;
		data_[i] = data;
	}

	numOfKeys_++;

	return true;
}

template<typename S>
bool BTreeNode<S>::addChild(BTreeNode<S>* child, int i)
{
	if (degree() == 2 * t())
	{
		return false;
	}

	if (i == -1)
	{
		children_[degree()] = child;
	}
	else
	{
		for (int j = degree(); j > i; j--)
		{
			children_[j] = children_[j - 1];
		}

		children_[i] = child;
	}

	degree_++;

	return true;
}

template<typename S>
bool BTreeNode<S>::removeKeyAndData(int i)
{
	if (i >= getNumOfKeys())
	{
		return false;
	}

	for (int j = i; j < getNumOfKeys() - 1; j++)
	{
		keys_[j] = keys_[j + 1];
		data_[j] = data_[j + 1];
	}

	numOfKeys_--;
	return true;
}

template<typename S>
bool BTreeNode<S>::removeChild(int i)
{
	if (i >= degree())
	{
		return false;
	}

	for (int j = i; j < degree() - 1; j++)
	{
		children_[j] = children_[j + 1];
	}

	degree_--;
	return true;
}
