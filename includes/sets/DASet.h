#pragma once

#include "Set.h"
#include "../lists/DynamicArray.h"

template <typename T>
class DASet : public Set<T>
{
public:
	DASet();
	~DASet();
	virtual int size();
	virtual bool add(T element);
	virtual bool contains(T element);
	virtual bool remove(T element);
	virtual T* elementsArray();
	virtual DASet<T>* unionSet(Set<T>& other);
	virtual DASet<T>* intersectionSet(Set<T>& other);
private:
	DynamicArray<T>* list_;
};

template<typename T>
inline DASet<T>::DASet()
{
	list_ = new DynamicArray<T>();
}

template<typename T>
inline DASet<T>::~DASet()
{
	delete list_;
}

template<typename T>
inline int DASet<T>::size()
{
	return list_->size();
}

template<typename T>
inline bool DASet<T>::add(T element)
{
	if (contains(element))
	{
		return false;
	}

	list_->add(element);

	return true;
}

template<typename T>
inline bool DASet<T>::contains(T element)
{
	return list_->contains(element);
}

template<typename T>
inline bool DASet<T>::remove(T element)
{
	return list_->remove(element);
}

template<typename T>
inline T* DASet<T>::elementsArray()
{
	T* arr = new T[size()];

	for (int i = 0; i < size(); i++)
	{
		arr[i] = list_->get(i);
	}

	return arr;
}

template<typename T>
inline DASet<T>* DASet<T>::unionSet(Set<T>& other)
{
	DASet<T>* set = new DASet<T>;
	T* otherElements = other.elementsArray();

	for (int i = 0; i < size(); i++)
	{
		set->list_->add(list_->get(i));
	}

	for (int i = 0; i < other.size(); i++)
	{
		set->add(otherElements[i]);
	}

	delete otherElements;
	return set;
}

template<typename T>
inline DASet<T>* DASet<T>::intersectionSet(Set<T>& other)
{
	DASet<T>* set = new DASet<T>;
	bool thisIsShorter = size() <= other.size();
	T* otherElements = other.elementsArray();
	
	if (thisIsShorter)
	{
		for (int i = 0; i < size(); i++)
		{
			if (other.contains(list_->get(i)))
			{
				set->list_->add(list_->get(i));
			}
		}
	}
	else
	{
		for (int i = 0; i < other.size(); i++)
		{
			if (contains(otherElements[i]))
			{
				set->list_->add(otherElements[i]);
			}
		}
	}

	delete otherElements;
	return set;
}
