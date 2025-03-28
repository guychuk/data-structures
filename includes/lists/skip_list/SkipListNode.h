#pragma once

#include <stdexcept>
#include "../DynamicArray.h"

template<typename T>
class SkipListNode
{
public:
	// Constructor

	SkipListNode<T>();

	SkipListNode<T>(T key);

	// Getters

	SkipListNode<T>* getNext(int level);

	SkipListNode<T>* getPrev(int level);

	int getDist(int level);

	int getHeight();

	T getKey();

	bool hasKey();

	// Setters

	void setNext(int level, SkipListNode<T>* next);

	void setPrev(int level, SkipListNode<T>* prev);

	void setDist(int level, int dist);

	void addLevel(SkipListNode<T>* prev, SkipListNode<T>* next);

	void removeHighestLevel();

private:
	DynamicArray<SkipListNode<T>*>* next_;

	DynamicArray<SkipListNode<T>*>* prev_;

	DynamicArray<int>* dist_;

	int height_;

	T key_;

	bool hasKey_;
};

template<typename T>
SkipListNode<T>::SkipListNode()
{
	next_ = new DynamicArray<SkipListNode<T>*>(2);

	prev_ = new DynamicArray<SkipListNode<T>*>(2);

	dist_ = new DynamicArray<int>(2);

	height_ = -1;

	hasKey_ = false;
}

template<typename T>
SkipListNode<T>::SkipListNode(T key) : SkipListNode<T>()
{
	key_ = key;
	
	hasKey_ = true;
}

template<typename T>
SkipListNode<T>* SkipListNode<T>::getNext(int level)
{
	if (level > getHeight())
	{
		throw std::out_of_range("level greater than node's height");
	}

	return next_->get(level);
}

template<typename T>
SkipListNode<T>* SkipListNode<T>::getPrev(int level)
{
	if (level > getHeight())
	{
		throw std::out_of_range("level greater than node's height");
	}

	return prev_->get(level);
}

template<typename T>
int SkipListNode<T>::getDist(int level)
{
	if (level > getHeight())
	{
		throw std::out_of_range("level greater than node's height");
	}

	return dist_->get(level);
}

template<typename T>
int SkipListNode<T>::getHeight()
{
	return height_;
}

template<typename T>
T SkipListNode<T>::getKey()
{
	if (!hasKey())
	{
		throw std::logic_error("this node's key is undefined");
	}

	return key_;
}

template<typename T>
bool SkipListNode<T>::hasKey()
{
	return hasKey_;
}

template<typename T>
void SkipListNode<T>::setNext(int level, SkipListNode<T>* next)
{
	if (level > getHeight())
	{
		throw std::out_of_range("level greater than node's height");
	}

	next_->set(level, next);
}

template<typename T>
void SkipListNode<T>::setPrev(int level, SkipListNode<T>* prev)
{
	if (level > getHeight())
	{
		throw std::out_of_range("level greater than node's height");
	}

	prev_->set(level, prev);
}

template<typename T>
void SkipListNode<T>::setDist(int level, int dist)
{
	if (level > getHeight())
	{
		throw std::out_of_range("level greater than node's height");
	}

	dist_->set(level, dist);
}

template<typename T>
void SkipListNode<T>::addLevel(SkipListNode<T>* prev, SkipListNode<T>* next)
{
	height_++;

	next_->add(next);

	prev_->add(prev);

	dist_->add(1);
}

template<typename T>
void SkipListNode<T>::removeHighestLevel()
{
	next_->removeLast();
	prev_->removeLast();
	dist_->removeLast();

	height_--;
}
