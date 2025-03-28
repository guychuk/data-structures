#pragma once

#include <stdexcept>

template <typename T>
struct Set
{
	virtual int size() = 0;
	virtual bool add(T element) = 0;
	virtual bool contains(T element) = 0;
	virtual bool remove(T element) = 0;
	virtual bool isEmpty();
	virtual T* elementsArray() = 0;

	virtual bool isSubset(Set<T>& other);
	virtual Set<T>* unionSet(Set<T>& other) = 0;
	virtual Set<T>* intersectionSet(Set<T>& other) = 0;

	class Iterator
	{
	public:
		Iterator(Set<T>& set);
		~Iterator();

		bool hasNext();
		T next();
		
	private:
		T* array_;
		int nextIndex_;
		int size_;
	};

	virtual Set<T>::Iterator iterator()
	{
		return Iterator(*this);
	}
};

template<typename T>
inline bool Set<T>::isEmpty()
{
	return size() == 0;
}

template<typename T>
inline bool Set<T>::isSubset(Set<T>& other)
{
	if (other.size() > size())
	{
		return false;
	}

	T* otherElements = other.elementsArray();

	bool subset = true;

	for (int i = 0; i < other.size() && subset; i++)
	{
		subset = contains(otherElements[i]);
	}

	delete otherElements;
	return subset;
}

template<typename T>
inline Set<T>::Iterator::Iterator(Set<T>& set)
{
	array_ = set.elementsArray();
	nextIndex_ = 0;
	size_ = set.size();
}

template<typename T>
inline Set<T>::Iterator::~Iterator()
{
	nextIndex_ = -1;
	size_ = -1;
	delete[] array_;
	array_ = nullptr;
}

template<typename T>
inline bool Set<T>::Iterator::hasNext()
{
	return nextIndex_ < size_;
}

template<typename T>
inline T Set<T>::Iterator::next()
{
	if (!hasNext())
	{
		throw std::logic_error("this iterator has no next element");
	}

	return array_[nextIndex_++];
}
