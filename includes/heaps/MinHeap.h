#pragma once

#include <stdexcept>
#include "../lists/DynamicArray.h"

template<typename T>
class MinHeap
{
public:
	/**
	 * @brief build a new minimum heap.
	 * @param capacity the maximal number of elements in the heap.
	*/
	MinHeap(const int capacity);

	/**
	 * @brief build a minimum heap from a given array.
	 * @param arr
	 * @param size the size of the array and eventually the size of the heap.
	*/
	MinHeap(T* arr, const int size);

	MinHeap(DynamicArray<T> arr);

	~MinHeap();

	/**
	 * @brief get the index of the parent of the element at index i.
	 * @param i
	 * @return the parent's index, may not be a valid index in the heap.
	*/
	int parent(int i);

	/**
	 * @brief get the index of the left child of the element at index i.
	 * @param i
	 * @return the left child's index, may not be a valid index in the heap.
	*/
	int left(int i);

	/**
	 * @brief get the index of the right child of the element at index i.
	 * @param i
	 * @return the right child's index, may not be a valid index in the heap.
	*/
	int right(int i);

	/**
	 * @brief get the minimum element in the heap keep it in the heap.
	 * @return the maximum number in the heap.
	*/
	T minimum();

	/**
	 * @brief get the maximum element in the heap and pop is out.
	 * @return the maximum number in the heap.
	*/
	T extractMin();

	/**
	 * @brief increase the key at a specific index.
	 * @param i
	 * @param key
	*/
	void decreaseKey(int i, T key);

	/**
	 * @brief insert a new key to the heap.
	 * @param key
	*/
	void insert(T key);

	/**
	 * @brief check if this heap is empty.
	 * @return true iff the heap is empty.
	*/
	bool isEmpty();

	/**
	 * @brief check if the heap is full.
	 * @return true iff the heap is full.
	*/
	bool isFull();

	/**
	 * @brief sort a given array using a min. heap (decreading).
	 * @param arr
	 * @param size the array's size.
	*/
	static void heapSort(T* arr, const int size);

	static void heapSort(DynamicArray<T> arr);

private:
	T* arr_;
	int capacity_;
	int size_;


	// used in insertion and in key increasing, marks the last element as positive inifinity
	bool lastIsPosInf_;

	/**
	 * @brief make sure that the subree rooted at index i satisfies the max. heap properties,
	 * assuming its left and right subtrees do too.
	 * @param i
	*/
	void MinHeapify(int i);
};

template<typename T>
inline MinHeap<T>::MinHeap(const int capacity)
{
	capacity_ = capacity;
	size_ = 0;
	arr_ = new T[capacity];
	lastIsPosInf_ = false;
}

template<typename T>
inline MinHeap<T>::MinHeap(T* arr, const int size)
{
	capacity_ = size;
	size_ = size;
	lastIsPosInf_ = false;

	arr_ = new T[capacity_];

	for (int i = 0; i < size; i++)
	{
		arr_[i] = arr[i];
	}

	// start at the deepest internal "node", then fix each "node" before it 
	for (int i = parent(size - 1); i >= 0; i--)
	{
		MinHeapify(i);
	}
}

template<typename T>
inline MinHeap<T>::MinHeap(DynamicArray<T> arr)
{
	capacity_ = arr.capacity();
	size_ = arr.size();
	lastIsPosInf_ = false;

	arr_ = new T[capacity_];

	for (int i = 0; i < size_; i++)
	{
		arr_[i] = arr[i];
	}

	// start at the deepest internal "node", then fix each "node" before it 
	for (int i = parent(size_ - 1); i >= 0; i--)
	{
		MinHeapify(i);
	}
}

template<typename T>
inline MinHeap<T>::~MinHeap()
{

	delete[] arr_;

	size_ = -1;
}

template<typename T>
inline int MinHeap<T>::parent(int i)
{
	if (i < 0)
	{
		throw std::invalid_argument("index cannot be negative");
	}

	if (i == 0)
	{
		throw std::logic_error("the first element does not have a parent");
	}

	if (i >= size_)
	{
		throw std::out_of_range("index out of range");
	}

	return (i - 1) / 2;
}

template<typename T>
inline int MinHeap<T>::left(int i)
{
	if (i < 0)
	{
		throw std::invalid_argument("index cannot be negative");
	}

	if (i >= size_)
	{
		throw std::out_of_range("index out of range");
	}

	return 2 * i + 1;
}

template<typename T>
inline int MinHeap<T>::right(int i)
{
	return left(i) + 1;
}

template<typename T>
inline T MinHeap<T>::minimum()
{
	if (isEmpty())
	{
		throw std::underflow_error("this heap is empty");
	}

	return arr_[0];
}

template<typename T>
inline T MinHeap<T>::extractMin()
{
	if (isEmpty())
	{
		throw std::underflow_error("this heap is empty");
	}

	T min = minimum();

	// move the last element to the beginning, then fix the heap

	if (!isEmpty())
	{
		arr_[0] = arr_[size_ - 1];
	}

	size_--;

	if (!isEmpty())
	{
		MinHeapify(0);
	}

	return min;
}

template<typename T>
inline void MinHeap<T>::decreaseKey(int i, T key)
{
	if (i >= size_)
	{
		throw std::out_of_range("index out of range");
	}

	// if we treat the last element as inifinity, no need to check
	// else, check that the key is really bigger
	if (!(lastIsPosInf_ && i == size_ - 1) && key > arr_[i])
	{
		throw std::logic_error("the new key is greater than the original key");
	}

	arr_[i] = key;

	lastIsPosInf_ = false;

	// while the current is smaller than its parent, swap them
	while (i && arr_[parent(i)] > arr_[i])
	{
		// swap the current and its parent
		T tmp = arr_[parent(i)];
		arr_[parent(i)] = arr_[i];
		arr_[i] = tmp;

		i = parent(i);
	}
}

template<typename T>
inline void MinHeap<T>::insert(T key)
{
	if (isFull())
	{
		throw std::overflow_error("this heap is full");
	}

	lastIsPosInf_ = true;
	size_++;

	decreaseKey(size_ - 1, key);
}

template<typename T>
inline bool MinHeap<T>::isEmpty()
{
	return size_ == 0;
}

template<typename T>
inline bool MinHeap<T>::isFull()
{
	return size_ == capacity_;
}

template<typename T>
inline void MinHeap<T>::heapSort(T* arr, const int size)
{
	MinHeap<T> heap(arr, size);

	for (int i = size - 1; i >= 0; i--)
	{
		arr[i] = heap.extractMin();
	}
}

template<typename T>
inline void MinHeap<T>::heapSort(DynamicArray<T> arr)
{
	MinHeap<T> heap(arr, arr.size());

	for (int i = arr.size() - 1; i >= 0; i--)
	{
		arr[i] = heap.extractMin();
	}
}

template<typename T>
inline void MinHeap<T>::MinHeapify(int i)
{
	if (i >= size_)
	{
		throw std::out_of_range("index out of range");
	}

	int leftIndex = left(i), rightIndex = right(i), smallest = i;

	// find where the smallest element between this node and its children

	if (leftIndex < size_ && arr_[leftIndex] < arr_[smallest])
	{
		smallest = leftIndex;
	}

	if (rightIndex < size_ && arr_[rightIndex] < arr_[smallest])
	{
		smallest = rightIndex;
	}

	// it this node is not the smallest, rearrange the array

	if (smallest != i)
	{
		// swap this and the larger child
		T tmp = arr_[smallest];
		arr_[smallest] = arr_[i];
		arr_[i] = tmp;

		// check the new subtree
		MinHeapify(smallest);
	}
}
