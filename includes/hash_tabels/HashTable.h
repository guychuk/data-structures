#pragma once
#include "../Pair.h"

static const int	DEFAULT_CAPACITY		= 64;
static const double DEFAULT_MAX_LOAD_FACTOR = 0.5;
static const double DEFAULT_MIN_LOAD_FACTOR = 0.25;
static const double DEFAULT_GROWTH_FACTOE	= 2.0;

template<typename K, typename S>
class HashTable
{
public:
	/**
	 * @brief insert a key and data to the table.
	 * @param key is the identifier of the data.
	 * @param data 
	*/
	virtual void insert(K key, S data) = 0;

	/**
	 * @brief change the value identified by some key.
	 * @param key the key that identifies thenew data.
	 * @param data 
	*/
	virtual void set(K key, S data) = 0;

	/**
	 * @brief search a pair of key and data in the table.
	 * @param key 
	 * @return a pointer to the pair containing the key and the data, or nullptr if the key is not in the table.
	*/
	virtual Pair<K, S>* search(K key) = 0;

	/**
	 * @brief rmove a pair from the table by a key.
	 * @param key 
	*/
	virtual void remove(K key) = 0;

	/**
	 * @return the number of elements in the table.
	*/
	virtual  int getNumOfElements();

	/**
	 * @return the size of the table.
	*/
	virtual int getSize();

	/**
	 * @return the maximal load factor.
	*/
	virtual double getMaxLoadFactor();

	/**
	 * @return the minimal load factor.
	*/
	virtual double getMinLoadFactor();

	/**
	 * @return the growth factor (by what number the table grows/shrinks when hitting the limits?).
	*/
	virtual double getGrowthFactor();

	/**
	 * @return the load factor (the ratio num. of elements / size).
	*/
	virtual double getLoadFactor();

	/**
	 * @brief check if the table is empty.
	 * @return true iff the table is empty.
	*/
	virtual bool isEmpty();

protected:
	double	maxLoadFactor_;
	double	minLoadFactor_;
	double	growthFactor_;
	int		numOfElements_;
	size_t		size_;

	std::hash<K>* hashFunc_;

	/**
	 * @brief create a bigger array and copy the elements to it.
	*/
	virtual void extend();

	/**
	 * @brief create a smaller array and copy the elements to it.
	*/
	virtual void shrink();

	/**
	 * @brief create a new array and copy the elements to it.
	 * @param newSize the new size.
	*/
	virtual void resize(int newSize) = 0;

	/**
	 * @brief a function to hash a key.
	 * @param key a hey to hash.
	 * @return a valid index in the table.
	*/
	virtual size_t hash(K key);
};

template<typename K, typename S>
inline int HashTable<K, S>::getNumOfElements()
{
	return numOfElements_;
}

template<typename K, typename S>
inline int HashTable<K, S>::getSize()
{
	return size_;
}

template<typename K, typename S>
inline double HashTable<K, S>::getMaxLoadFactor()
{
	return maxLoadFactor_;
}

template<typename K, typename S>
inline double HashTable<K, S>::getMinLoadFactor()
{
	return minLoadFactor_;
}

template<typename K, typename S>
inline double HashTable<K, S>::getGrowthFactor()
{
	return growthFactor_;
}

template<typename K, typename S>
inline double HashTable<K, S>::getLoadFactor()
{
	return (double)(numOfElements_) / (double)(size_);
}

template<typename K, typename S>
inline bool HashTable<K, S>::isEmpty()
{
	return numOfElements_ == 0;
}

template<typename K, typename S>
inline void HashTable<K, S>::extend()
{
	resize((int)(this->getSize() * this->getGrowthFactor()));
}

template<typename K, typename S>
inline void HashTable<K, S>::shrink()
{
	resize((int)(this->getSize() / this->getGrowthFactor()));
}

template<typename K, typename S>
inline size_t HashTable<K, S>::hash(K key)
{
	return (*hashFunc_)(key) % getSize();
}
