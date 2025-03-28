#pragma once

#include "lists/DynamicArray.h"

template <typename T>
class Sort
{
public:
	static void bubbleSort(T* arr, int size);

	static void bubbleSort(DynamicArray<T>& arr);

	static void insertionSort(T* arr, int size);

	static void insertionSort(DynamicArray<T>& arr);

	static void selectionSort(T* arr, int size);

	static void selectionSort(DynamicArray<T>& arr);
};

template<typename T>
void Sort<T>::bubbleSort(T* arr, int size)
{
	bool finished;
	
	do
	{
		finished = true;

		for (int i = 0; i < size - 1; i++)
		{
			if (arr[i] > arr[i + 1])
			{
				T tmp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = tmp;
				finished = false;
			}
		}
	} while (!finished);
}

template<typename T>
inline void Sort<T>::bubbleSort(DynamicArray<T>& arr)
{
	bubbleSort(arr.array_, arr.size());
}

template<typename T>
inline void Sort<T>::insertionSort(T* arr, int size)
{
	int j;

	for (int i = 1; i < size; i++)
	{
		j = i;

		while (j > 0 && arr[j] < arr[j - 1])
		{
			T tmp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = tmp;
			j--;
		}
	}
}

template<typename T>
inline void Sort<T>::insertionSort(DynamicArray<T>& arr)
{
	insertionSort(arr.array_, arr.size());
}

template<typename T>
inline void Sort<T>::selectionSort(T* arr, int size)
{
	int minIndex;

	for (int sorted = 0; sorted < size; sorted++)
	{
		minIndex = sorted;

		for (int i = sorted + 1; i < size; i++)
		{
			if (arr[i] < arr[minIndex])
			{
				minIndex = i;
			}
		}

		T tmp = arr[sorted];
		arr[sorted] = arr[minIndex];
		arr[minIndex] = tmp;
	}
}

template<typename T>
inline void Sort<T>::selectionSort(DynamicArray<T>& arr)
{
	selectionSort(arr.array_, arr.size());
}
