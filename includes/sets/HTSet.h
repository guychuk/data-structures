#pragma once

#include "../lists/linked_lists/DLinkedList.h"
#include "../hash_tabels/LPHashTable.h"
#include "Set.h"

template <typename T>
class HTSet : public Set<T>
{
public:
	HTSet();
	~HTSet();
	virtual int size();
	virtual bool add(T element);
	virtual bool contains(T element);
	virtual bool remove(T element);
	virtual T* elementsArray();
	virtual HTSet<T>* unionSet(Set<T>& other);
	virtual HTSet<T>* intersectionSet(Set<T>& other);
private:
	DLinkedList<T>* list_;
	LPHashTable<T, DNode<T>*>* table_;
	void addNoSearch(T element);
};

template<typename T>
inline HTSet<T>::HTSet()
{
	list_ = new DLinkedList<T>;
	table_ = new LPHashTable<T, DNode<T>*>;
}

template<typename T>
inline HTSet<T>::~HTSet()
{
	delete list_;
	delete table_;
}

template<typename T>
inline int HTSet<T>::size()
{
	return list_->size();
}

template<typename T>
inline bool HTSet<T>::add(T element)
{
	if (contains(element))
	{
		return false;
	}

	addNoSearch(element);

	return true;
}

template<typename T>
inline bool HTSet<T>::contains(T element)
{
	return table_->search(element) != nullptr;
}

template<typename T>
inline bool HTSet<T>::remove(T element)
{
	Pair<T, DNode<T>*>* searchResult = table_->search(element);
	bool removedFromList;

	if (!searchResult)
	{
		return false;
	}

	removedFromList = list_->remove(searchResult->getData());

	if (removedFromList)
	{
		table_->remove(element);
	}

	return removedFromList;
}

template<typename T>
inline T* HTSet<T>::elementsArray()
{
	T* arr = new T[size()];

	DNode<T>* curr = list_->getNode(0);

	for (int i = 0; i < size(); i++)
	{
		arr[i] = curr->data();
		curr = curr->next;
	}

	return arr;
}

template<typename T>
inline HTSet<T>* HTSet<T>::unionSet(Set<T>& other)
{
	HTSet<T>* set = new HTSet<T>;
	T* otherElements = other.elementsArray();
	DNode<T>* curr = list_->getNode(0);

	for (int i = 0; i < size(); i++)
	{
		set->addNoSearch(curr->data());
		curr = curr->next;
	}

	for (int i = 0; i < other.size(); i++)
	{
		set->add(otherElements[i]);
	}

	delete otherElements;
	return set;
}

template<typename T>
inline HTSet<T>* HTSet<T>::intersectionSet(Set<T>& other)
{
	HTSet<T>* set = new HTSet<T>;
	bool thisIsShorter = size() <= other.size();
	T* otherElements = other.elementsArray();
	DNode<T>* curr;

	if (thisIsShorter)
	{
		curr = list_->getNode(0);

		for (int i = 0; i < size(); i++)
		{
			if (other.contains(curr->data()))
			{
				set->addNoSearch(curr->data());
			}

			curr = curr->next;
		}
	}
	else
	{
		for (int i = 0; i < other.size(); i++)
		{
			if (contains(otherElements[i]))
			{
				set->addNoSearch(otherElements[i]);
			}
		}
	}

	delete otherElements;
	return set;
}

template<typename T>
inline void HTSet<T>::addNoSearch(T element)
{
	list_->addLast(element);
	table_->insert(element, list_->getNode(0)->prev->prev);
}
