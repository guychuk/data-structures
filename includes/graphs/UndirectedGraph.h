#pragma once

#include "Graph.h"

template<typename T>
struct UndirectedGraph : public Graph<T>
{
	// from Graph class

	virtual int numOfVerices() = 0;
	virtual int numOfEdges() = 0;
	virtual bool hasVertex(T vertex) = 0;
	virtual bool hasEdge(T vertex1, T vertex2) = 0;
	virtual bool hasEulerianCycle() = 0;
	virtual void addEdge(T vertex1, T vertex2) = 0;
	virtual void removeEdge(T vertex1, T vertex2) = 0;

	// degrees

	virtual int degreeOf(T vertex) = 0;
	virtual int maxDegree() = 0;
	virtual int minDegree() = 0;

	// conncetivity

	virtual bool isConnceted() = 0;
};