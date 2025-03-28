#pragma once

#include <stdexcept>

template<typename T>
class MaxHeap 
{
public:
	/**
	 * @brief build a new maximum heap.
	 * @param capacity the maximal number of elements in the heap.
	*/
	MaxHeap(const int capacity);

	/**
	 * @brief build a maximum heap from a given array.
	 * @param arr 
	 * @param size the size of the array and eventually the size of the heap.
	*/
	MaxHeap(T* arr, const int size);

	~MaxHeap();

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
	 * @brief get the maximum element in the heap keep it in the heap.
	 * @return the maximum number in the heap.
	*/
	T maximum();

	/**
	 * @brief get the maximum element in the heap and pop is out.
	 * @return the maximum number in the heap.
	*/
	T extractMax();

	/**
	 * @brief increase the key at a specific index.
	 * @param i 
	 * @param key 
	*/
	void increaseKey(int i, T key);

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
	 * @brief sort a given array using a max. heap (increasing).
	 * @param arr 
	 * @param size the array's size.
	*/
	static void heapSort(T* arr, const int size);

private:
	T* arr_;
	int capacity_;
	int size_;


	// used in insertion and in key increasing, marks the last element as negative inifinity
	bool lastIsNegInf_;	

	/**
	 * @brief make sure that the subree rooted at index i satisfies the max. heap properties, 
	 * assuming its left and right subtrees do too.
	 * @param i 
	*/
	void maxHeapify(int i);
};

template<typename T>
inline MaxHeap<T>::MaxHeap(const int capacity)
{
	capacity_ = capacity;
	size_ = 0;
	arr_ = new T[capacity];
	lastIsNegInf_ = false;
}

template<typename T>
inline MaxHeap<T>::MaxHeap(T* arr, const int size)
{
	capacity_ = size;
	size_ = size;
	lastIsNegInf_ = false;

	arr_ = new T[size];

	for (int i = 0; i < size; i++)
	{
		arr_[i] = arr[i];
	}

	if (size > 1)
	{
		// start at the deepest internal "node", then fix each "node" before it 
		for (int i = parent(size - 1); i >= 0; i--)
		{
			maxHeapify(i);
		}
	}
}

template<typename T>
inline MaxHeap<T>::~MaxHeap()
{
	delete[] arr_;

	size_ = -1;
}

template<typename T>
inline int MaxHeap<T>::parent(int i)
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
inline int MaxHeap<T>::left(int i)
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
inline int MaxHeap<T>::right(int i)
{
	return left(i) + 1;
}

template<typename T>
inline T MaxHeap<T>::maximum()
{
	if (isEmpty())
	{
		throw std::underflow_error("this heap is empty");
	}

	return arr_[0];
}

template<typename T>
inline T MaxHeap<T>::extractMax()
{
	if (isEmpty())
	{
		throw std::underflow_error("this heap is empty");
	}

	T max = maximum();

	// move the last element to the beginning, then fix the heap

	if (!isEmpty())
	{
		arr_[0] = arr_[size_ - 1];
	}

	size_--;

	if (!isEmpty())
	{
		maxHeapify(0);
	}

	return max;
}

template<typename T>
inline void MaxHeap<T>::increaseKey(int i, T key)
{
	if (i >= size_)
	{
		throw std::out_of_range("index out of range");
	}
	
	// if we treat the last element as inifinity, no need to check
	// else, check that the key is really bigger
	if (!(lastIsNegInf_ && i == size_ - 1) && key < arr_[i])
	{
		throw std::logic_error("the new key is smaller than the original key");
	}

	arr_[i] = key;

	lastIsNegInf_ = false;

	// while the current is greater than its parent, swap them
	while (i && arr_[parent(i)] < arr_[i])
	{
		// swap the current and its parent
		T tmp = arr_[parent(i)];
		arr_[parent(i)] = arr_[i];
		arr_[i] = tmp;

		i = parent(i);
	}
}

template<typename T>
inline void MaxHeap<T>::insert(T key)
{
	if (isFull())
	{
		throw std::overflow_error("this heap is full");
	}

	lastIsNegInf_ = true;
	size_++;

	increaseKey(size_ - 1, key);
}

template<typename T>
inline bool MaxHeap<T>::isEmpty()
{
	return size_ == 0;
}

template<typename T>
inline bool MaxHeap<T>::isFull()
{
	return size_ == capacity_;
}

template<typename T>
inline void MaxHeap<T>::heapSort(T* arr, const int size)
{
	MaxHeap<T> heap(arr, size);

	for (int i = size - 1; i >= 0; i--)
	{
		arr[i] = heap.extractMax();
	}
}

template<typename T>
inline void MaxHeap<T>::maxHeapify(int i)
{
	if (i >= size_)
	{
		throw std::out_of_range("index out of range");
	}

	int leftIndex = left(i), rightIndex = right(i), largest = i;

	// find where the largest element between this node and its children

	if (leftIndex < size_ && arr_[leftIndex] > arr_[largest])
	{
		largest = leftIndex;
	}

	if (rightIndex < size_ && arr_[rightIndex] > arr_[largest])
	{
		largest = rightIndex;
	}

	// it this node is not the largest, rearrange the array

	if (largest != i)
	{
		// swap this and the larger child
		T tmp = arr_[largest];
		arr_[largest] = arr_[i];
		arr_[i] = tmp;

		// check the new subtree
		maxHeapify(largest);
	}
}
