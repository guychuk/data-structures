#pragma once

#include "HashTable.h"
#include "../lists/linked_lists/DNode.h"

template<typename K, typename S>
class ChainedHashTable : public HashTable<K, S>
{
public:
	/**
	 * @brief initialize a new hash table.
	 * @param initSize is the initial size of the table.
	 * @param maxLoadFactor is the maximal load factor for the table.
	 * @param minLoadFactor is the minimal load factor for the table.
	 * @param growthFactor is the growth factor of the table.
	 * @note use -1 to use the default value for each argument.
	*/
	ChainedHashTable(int initSize = -1, double maxLoadFactor = -1.0, double minLoadFactor = -1.0, double growthFactor = -1.0);

	~ChainedHashTable();

	void insert(K key, S data);

	void set(K key, S data);

	Pair<K, S>* search(K key);

	S get(K key);

	void remove(K key);

protected:
	DNode<Pair<K, S>*>** arr;	// an array of pointer to doubly linked lists nodes

	void resize(int newSize);

	/**
	 * @brief search a key in the table.
	 * @param key 
	 * @return the node that contains the key or nullptr if there's no such node.
	*/
	DNode<Pair<K, S>*>* searchNode(K key);
};

template<typename K, typename S>
inline ChainedHashTable<K, S>::ChainedHashTable(int initCapacity, double maxLoadFactor, double minLoadFactor, double growthFactor)
{
	if (initCapacity <= 0 && initCapacity != -1)
	{
		throw std::invalid_argument("initial capacity should be a positive number");
	}

	if (maxLoadFactor <= 0 && maxLoadFactor != -1)
	{
		throw std::invalid_argument("max. load factor should be a positive number");
	}

	if (minLoadFactor < 0 && minLoadFactor != -1)
	{
		throw std::invalid_argument("min. load factor should be a non-negative number");
	}

	if (growthFactor <= 1 && growthFactor != -1)
	{
		throw std::invalid_argument("growth factor should be greater than 1");
	}

	this->size_				= initCapacity	== -1	? DEFAULT_CAPACITY			: initCapacity;
	this->maxLoadFactor_	= maxLoadFactor == -1	? DEFAULT_MAX_LOAD_FACTOR	: maxLoadFactor;
	this->minLoadFactor_	= minLoadFactor == -1	? DEFAULT_MIN_LOAD_FACTOR	: minLoadFactor;
	this->growthFactor_		= growthFactor	== -1	? DEFAULT_GROWTH_FACTOE		: growthFactor;

	arr = new DNode<Pair<K, S>*>*[this->size_];

	// initialize the table
	for (int i = 0; i < this->size_; i++)
	{
		arr[i] = nullptr;
	}

	this->hashFunc_ = new std::hash<K>;
}

template<typename K, typename S>
inline ChainedHashTable<K, S>::~ChainedHashTable()
{
	DNode<Pair<K, S>*>* currentNode = nullptr, * nextNode = nullptr;

	// scan all the lists
	for (int i = 0; i < this->size_; i++)
	{
		currentNode = arr[i];

		while (currentNode)
		{
			nextNode = currentNode->next;

			delete currentNode;

			currentNode = nextNode;
		}
	}

	delete[] arr;

	this->size_ = -1;
}

template<typename K, typename S>
inline void ChainedHashTable<K, S>::insert(K key, S data)
{
	DNode<Pair<K, S>*>* searchResult = searchNode(key), *newNode = nullptr;
	
	size_t index = 0;

	if (searchResult != nullptr)
	{
		throw std::logic_error("this key is aleady pointing to an object; consider using the set(K) function");
	}

	newNode = new DNode<Pair<K, S>*>(new Pair<K, S>(key, data));

	index = this->hash(key);

	// if the list that should contain the new pair does not exist, make the new node its head
	if (arr[index] == nullptr)
	{
		arr[index] = newNode;
	}
	else
	{
		// put the new node at the beginning of the list
		newNode->next = arr[index];
		arr[index]->prev = newNode;
		arr[index] = newNode;
	}
	
	this->numOfElements_++;

	// rehash
	if (this->getLoadFactor() > this->getMaxLoadFactor())
	{
		this->extend();
	}
}

template<typename K, typename S>
inline void ChainedHashTable<K, S>::set(K key, S data)
{
	Pair<K, S>* searchResult = search(key);

	if (searchResult == nullptr)
	{
		throw std::logic_error("this key does not exist");
	}

	searchResult->setData(data);
}

template<typename K, typename S>
Pair<K, S>* ChainedHashTable<K, S>::search(K key)
{
	DNode<Pair<K, S>*>* searchResult = searchNode(key);

	return searchResult ? searchNode(key)->data() : nullptr;
}

template<typename K, typename S>
inline S ChainedHashTable<K, S>::get(K key)
{
	Pair<K, S>* searchResult = search(key);

	if (searchResult == nullptr)
	{
		throw std::invalid_argument("key not found");
	}

	return searchResult->getData();
}

template<typename K, typename S>
inline void ChainedHashTable<K, S>::remove(K key)
{
	DNode<Pair<K, S>*>* node = searchNode(key);
	size_t index = -1;

	if (node == nullptr)
	{
		throw std::logic_error("this key does not exists");
	}

	if (node->prev && node->next)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	else if (node->prev)
	{
		// node is the tail
		node->prev->next = nullptr;
	}
	else
	{
		// node is the head

		index = this->hash(key);

		if (node->next)
		{
			node->next->prev = nullptr;
		}

		arr[index] = node->next;
	}

	delete node;

	this->numOfElements_--;

	if (this->getLoadFactor() < this->getMinLoadFactor())
	{
		this->shrink();
	}
}

template<typename K, typename S>
inline void ChainedHashTable<K, S>::resize(int newSize)
{
	int oldCapacity = this->getSize();

	this->size_ = newSize;	// change the capacity before hashing because the hash function uses the capacity

	DNode<Pair<K, S>*>** newArr = new DNode<Pair<K, S>*>* [newSize];

	DNode<Pair<K, S>*>* currentNode = nullptr, *nextNode = nullptr;

	size_t index = -1;

	// initialize the new array

	for (int i = 0; i < newSize; i++)
	{
		newArr[i] = nullptr;
	}

	// scan all the lists
	for (int i = 0; i < oldCapacity; i++)
	{
		currentNode = arr[i];

		while (currentNode)
		{
			nextNode = currentNode->next;

			index = this->hash(currentNode->data()->getKey());
	
			// make the current node the head
			currentNode->next = newArr[index];
			currentNode->prev = nullptr;
			newArr[index] = currentNode;

			currentNode = nextNode;
		}
	}

	delete[] arr;
	arr = newArr;
}

template<typename K, typename S>
inline DNode<Pair<K, S>*>* ChainedHashTable<K, S>::searchNode(K key)
{
	DNode<Pair<K, S>*>* currentNode = arr[this->hash(key)];

	while (currentNode && currentNode->data()->getKey() != key)
	{
		currentNode = currentNode->next;
	}

	return currentNode;
}
