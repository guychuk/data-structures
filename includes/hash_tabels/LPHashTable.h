#pragma once

#include "HashTable.h"

template<typename K, typename S>
class LPHashTable : public HashTable<K, S>
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
	LPHashTable(int initSize = -1, double maxLoadFactor = -1.0, double minLoadFactor = -1.0, double growthFactor = -1.0);

	~LPHashTable();

	void insert(K key, S data);

	void set(K key, S data);

	Pair<K, S>* search(K key);

	S get(K key);

	void remove(K key);

protected:
	Pair<K, S>** arr;	// an array of pointer to pair

	void resize(int newSize);

	size_t searchIndex(K key);

	void insertWithoutSearch(K key, S data);

	void insertWithoutSearch(Pair<K, S>* newPair);
};

template<typename K, typename S>
inline LPHashTable<K, S>::LPHashTable(int initSize, double maxLoadFactor, double minLoadFactor, double growthFactor)
{
	if (initSize <= 0 && initSize != -1)
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

	this->size_				= initSize		== -1 ? DEFAULT_CAPACITY		: initSize;
	this->maxLoadFactor_	= maxLoadFactor == -1 ? DEFAULT_MAX_LOAD_FACTOR : maxLoadFactor;
	this->minLoadFactor_	= minLoadFactor == -1 ? DEFAULT_MIN_LOAD_FACTOR : minLoadFactor;
	this->growthFactor_		= growthFactor	== -1 ? DEFAULT_GROWTH_FACTOE	: growthFactor;

	arr = new Pair<K, S>* [this->size_];

	// initialize the table
	for (unsigned i = 0; i < this->size_; i++)
	{
		arr[i] = nullptr;
	}

	this->hashFunc_ = new std::hash<K>;
}

template<typename K, typename S>
inline LPHashTable<K, S>::~LPHashTable()
{
	for (unsigned i = 0; i < this->size_; i++)
	{
		delete arr[i];
	}

	delete[] arr;

	this->size_ = -1;
}

template<typename K, typename S>
inline void LPHashTable<K, S>::insert(K key, S data)
{
	if (search(key))
	{
		throw std::logic_error("this key is aleady pointing to an object; consider using the set(K) function");
	}

	insertWithoutSearch(key, data);
	this->numOfElements_++;

	// rehash
	if (this->getLoadFactor() > this->getMaxLoadFactor())
	{
		this->extend();
	}
}

template<typename K, typename S>
inline void LPHashTable<K, S>::set(K key, S data)
{
	Pair<K, S>* searchResult = search(key);

	if (searchResult == nullptr)
	{
		throw std::logic_error("this key does not exist");
	}

	searchResult->setData(data);
}

template<typename K, typename S>
inline Pair<K, S>* LPHashTable<K, S>::search(K key)
{
	size_t indexFound = searchIndex(key);

	return indexFound < this->size_ ? arr[indexFound] : nullptr;
}

template<typename K, typename S>
inline S LPHashTable<K, S>::get(K key)
{
	Pair<K, S>* searchResult = search(key);

	if (searchResult == nullptr)
	{
		throw std::invalid_argument("key not found");
	}

	return searchResult->getData();
}

template<typename K, typename S>
inline void LPHashTable<K, S>::remove(K key)
{
	size_t indexFound = searchIndex(key);
	Pair<K, S>* current;

	if (indexFound == this->size_)
	{
		throw std::logic_error("this key does not exists");
	}

	// delete the desired item
	delete arr[indexFound];
	arr[indexFound] = nullptr;
	this->numOfElements_--;

	// until reaching nullptr, rehash the elements
	current = arr[++indexFound];

	while (current)
	{
		arr[indexFound] = nullptr;
		insertWithoutSearch(current);
		current = arr[++indexFound];
	}

	// rehash
	if (this->getLoadFactor() < this->getMinLoadFactor())
	{
		this->shrink();
	}
}

template<typename K, typename S>
inline void LPHashTable<K, S>::resize(int newSize)
{
	int oldSize = this->size_;

	this->size_ = newSize;

	Pair<K, S>** oldArr = arr;

	arr = new Pair<K, S>* [newSize];

	// initialize the table
	for (int i = 0; i < newSize; i++)
	{
		arr[i] = nullptr;
	}

	for (int i = 0; i < oldSize; i++)
	{
		if (oldArr[i])
		{
			insertWithoutSearch(oldArr[i]);
		}
	}

	delete oldArr;
}

template<typename K, typename S>
inline size_t LPHashTable<K, S>::searchIndex(K key)
{
	size_t index = this->hash(key), offset = 0;
	Pair<K, S>* pair;

	while (offset < this->size_)
	{
		pair = this->arr[(index + offset) % this->size_];

		if (!pair)
		{
			return this->size_;
		}

		if (pair->getKey() == key)
		{
			return (index + offset) % this->size_;
		}

		offset++;
	}

	return this->size_;
}

template<typename K, typename S>
inline void LPHashTable<K, S>::insertWithoutSearch(K key, S data)
{
	Pair<K, S>* newPair = new Pair<K, S>(key, data);

	insertWithoutSearch(newPair);
}

template<typename K, typename S>
inline void LPHashTable<K, S>::insertWithoutSearch(Pair<K, S>* newPair)
{
	size_t index = this->hash(newPair->getKey()), offset = 0;

	// find the next available index
	while (offset < this->size_ && arr[(index + offset) % this->size_])
	{
		offset++;
	}

	if (offset == this->size_)
	{
		throw std::overflow_error("table overflow");
	}

	arr[(index + offset) % this->size_] = newPair;
}
