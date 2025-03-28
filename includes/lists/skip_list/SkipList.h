#pragma once

#include "SkipListNode.h"
#include <random>
#include <iostream>
#include <climits>

template<typename T>
class SkipList
{
public:
	// Constructor

	SkipList<T>(double prob = 0.5);

	// Operations

	SkipListNode<T>* find(T key);

	SkipListNode<T>* search(T key);

	SkipListNode<T>* insert(T key);

	bool remove(T key);

	T predecessor(T key);

	T successor(T Key);

	T minimum();

	T maximum();

	int rank(T key);

	T select(int index);

	// Getters

	bool isEmpty();

	void printList();

	void printLevel(int level);

	int size();

private:
	SkipListNode<T>* head_;

	SkipListNode<T>* tail_;

	int size_;

	double prob_;

	void increaseHeight();

	void decreaseHeight();

	int generateHeight();
};

template<typename T>
SkipList<T>::SkipList(double prob)
{
	head_ = new SkipListNode<T>();

	tail_ = new SkipListNode<T>();

	size_ = 0;

	if (prob <= 0 || prob >= 1)
	{
		throw std::invalid_argument("cannot have non-positive probability");
	}

	prob_ = prob;

	increaseHeight();
}

template<typename T>
void SkipList<T>::increaseHeight()
{
	head_->addLevel(nullptr, tail_);

	tail_->addLevel(head_, nullptr);
}

template<typename T>
void SkipList<T>::decreaseHeight()
{
	head_->removeHighestLevel();

	tail_->removeHighestLevel();
}

template<typename T>
int SkipList<T>::generateHeight()
{
	const int limit = INT_MAX;
	int counter = 1;

	std::random_device rnd;
	std::mt19937 twister(rnd());
	std::uniform_int_distribution<> dist(0, limit);

	while (dist(twister) <= limit * prob_)
	{
		counter++;
	}

	return counter;
}

template<typename T>
SkipListNode<T>* SkipList<T>::find(T key)
{
	SkipListNode<T>* node = search(key);

	return (isEmpty() || node->getKey() != key) ? nullptr : node;
}

template<typename T>
SkipListNode<T>* SkipList<T>::search(T key)
{
	SkipListNode<T>* node = head_;

	for (int level = head_->getHeight(); 0 <= level; level--)
	{
		while (node->getNext(level) && node->getNext(level) != tail_ && node->getNext(level)->getKey() <= key)
		{
			node = node->getNext(level);
		}
	}

	if (node == nullptr || node == head_ || node == tail_)
	{
		return nullptr;
	}

	return node;
}

template<typename T>
SkipListNode<T>* SkipList<T>::insert(T key)
{
	int nodeHeight = generateHeight(), oldHeight = head_->getHeight();
	
	int distanceAtLevel = 0;

	SkipListNode<T>* prevNode = nullptr, *nextNode = nullptr, *newNode = nullptr;

	SkipListNode<T>* current = nullptr, *shorter = nullptr, *prevAtLevel = nullptr;

	// if the new node is taller than all the existing nodes, increase the list's height
	for (int i = oldHeight; i < nodeHeight; i++)
	{
		increaseHeight();
	}

	// if the height of the list increased, update the distances between the sentinels
	if (oldHeight < nodeHeight)
	{
		for (int i = oldHeight + 1; i <= nodeHeight; i++)
		{
			// the distance between the sentinels is the num. of elements (size) + 1
			head_->setDist(i, size_ + 1);
		}
	}

	prevNode = search(key);

	if (prevNode == nullptr)
	{
		prevNode = head_;
	}
	// the key is already in the list
	else if (prevNode->getKey() == key)
	{
		return nullptr;
	}

	newNode = new SkipListNode<T>(key);

	for (int level = 0; level <= nodeHeight && prevNode; level++)
	{
		nextNode =  prevNode->getNext(level);

		newNode->addLevel(prevNode, nextNode);
		prevNode->setNext(level, newNode);
		nextNode->setPrev(level, newNode);

		// if the current level is the max. level of prevNode, move left until arriving at a higher node or the sentinel
		while (prevNode && prevNode->getHeight() == level)
		{
			prevNode = prevNode->getPrev(level);
		}
	}

	/** UPDATE THE DISTANCES */

	newNode->setDist(0, 1);

	// update the distances for each level from level 1 to the top
	for (int level = 1; level <= nodeHeight; level++)
	{
		distanceAtLevel = 0;
		current = newNode;

		// walk on the level beneath the current level
		// the jumps/skips are longer at higher levels
		while (current != newNode->getNext(level))
		{
			distanceAtLevel += current->getDist(level - 1);
			current = current->getNext(level - 1);
		}

		newNode->setDist(level, distanceAtLevel);
	}

	// update the distances in the nodes before the new one

	newNode->getPrev(0)->setDist(0, 1);

	// update the distances of the nodes before the new one, up to the height of the new node
	for (int level = 1; level <= nodeHeight; level++)
	{
		prevAtLevel = newNode->getPrev(level);

		// put the new node in the middle of the prev and the then-next of the prev node
		prevAtLevel->setDist(level, prevAtLevel->getDist(level) - newNode->getDist(level) + 1);
	}

	// update the distances of the taller nodes before the new one

	current = newNode;
	shorter = newNode;

	// the head is always the highest node
	while (current != head_)
	{
		// find the first (but we go backwards) node higher than the current node
		while (current != head_ && current->getHeight() <= shorter->getHeight())
		{
			current = current->getPrev(current->getHeight());
		}

		// add one to the distance in each layer of the node above the new one
		for (int level = shorter->getHeight() + 1; level <= current->getHeight(); level++)
		{
			current->setDist(level, current->getDist(level) + 1);
		}

		shorter = current;
	}

	size_++;

	return newNode;
}

template<typename T>
bool SkipList<T>::remove(T key)
{
	SkipListNode<T>* node = find(key), *prev = nullptr, * next = nullptr;
	SkipListNode<T>* current = nullptr, * shorter = nullptr;

	if (!node)
	{
		return false;
	}

	for (int level = 0; level <= node->getHeight(); level++)
	{
		prev = node->getPrev(level);
		next = node->getNext(level);

		prev->setNext(level, next);
		next->setPrev(level, prev);

		prev->setDist(level, prev->getDist(level) + node->getDist(level) - 1);
	}

	// update the distances of the taller nodes before the new one

	current = node;
	shorter = node;

	// the head is always the highest node
	while (current != head_)
	{
		// find the first (but we go backwards) node higher than the current node
		while (current != head_ && current->getHeight() <= shorter->getHeight())
		{
			current = current->getPrev(current->getHeight());
		}

		// subtract one to the distance in each layer of the node above the new one
		for (int level = shorter->getHeight() + 1; level <= current->getHeight(); level++)
		{
			current->setDist(level, current->getDist(level) - 1);
		}

		shorter = current;
	}

	size_--;

	while (head_->getNext(head_->getHeight()) == tail_ && !(size_ == 0 && head_->getHeight() == 0))
	{
		decreaseHeight();
	}

	return true;
}

template<typename T>
T SkipList<T>::predecessor(T key)
{
	SkipListNode<T>* node = find(key);

	if (!node)
	{
		throw std::invalid_argument("this key is not in the list");
	}

	if (node->getPrev(0) == head_)
	{
		throw std::out_of_range("this node has no predecessor");
	}

	return node->getPrev(0)->getKey();
}

template<typename T>
T SkipList<T>::successor(T key)
{
	SkipListNode<T>* node = find(key);

	if (!node)
	{
		throw std::invalid_argument("this key is not in the list");
	}

	if (node->getNext(0) == tail_)
	{
		throw std::out_of_range("this node has no successor");
	}

	return node->getNext(0)->getKey();
}

template<typename T>
T SkipList<T>::minimum()
{
	if (isEmpty())
	{
		throw std::logic_error("the list is empty");
	}

	return head_->getNext(0)->getKey();
}

template<typename T>
T SkipList<T>::maximum()
{
	if (isEmpty())
	{
		throw std::logic_error("the list is empty");
	}

	return tail_->getPrev(0)->getKey();
}

template<typename T>
int SkipList<T>::rank(T key)
{
	SkipListNode<T>* node = head_;
	int rank = 0;

	// start at the highet leftmost position in the list
	for (int level = head_->getHeight(); 0 <= level; level--)
	{
		// while the next node is less than or is the value we look for,
		// walk on that layer and sum the distances
		while (node->getNext(level) != nullptr && node->getNext(level)->getKey() <= key) {
			rank += node->getDist(level);
			node = node->getNext(level);
		}

		// then move one layer down
	}

	return rank;
}

template<typename T>
T SkipList<T>::select(int index)
{
	if (index >= size_)
	{
		throw std::out_of_range("index out of range");
	}

	SkipListNode<T>* current = head_;

	int level = current->getHeight(), leftToWalk = index;

	while (leftToWalk)
	{
		// while the next step will bring us to the desired place, or before that,
		// do it and add the distance to the counter
		while (current->getDist(level) <= leftToWalk)
		{
			leftToWalk -= current->getDist(level);
			current = current->getNext(level);
		}

		// when the next step is too long, move a layer down
		level--;
	}

	return current->getKey();
}

template<typename T>
bool SkipList<T>::isEmpty()
{
	return size_ == 0;
}

template<typename T>
void SkipList<T>::printList()
{
	for (int i = head_->getHeight(); i >= 0; i--)
	{
		printLevel(i);
		std::cout << std::endl;
	}
}

template<typename T>
void SkipList<T>::printLevel(int level)
{
	SkipListNode<T>* current = head_->getNext(level);

	std::cout << "H" << std::string(head_->getDist(level), '\t');

	while (current != tail_)
	{
		std::cout << current->getKey() << std::string(current->getDist(level), '\t');
		current = current->getNext(level);
	}

	std::cout << "T";
}

template<typename T>
inline int SkipList<T>::size()
{
	return size_;
}
