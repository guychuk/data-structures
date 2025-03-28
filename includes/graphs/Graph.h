#pragma once

#include "../sets/Set.h"

template <typename T>
struct Graph
{
	virtual int numOfVerices() = 0;
	virtual int numOfEdges() = 0;
	virtual bool hasVertex(T vertex) = 0;
	virtual bool hasEdge(T vertex1, T vertex2) = 0;
	virtual bool hasEulerianCycle() = 0;
	virtual bool loopsAllowed();
	virtual void addEdge(T vertex1, T vertex2) = 0;
	virtual void removeEdge(T vertex1, T vertex2) = 0;
	
	int V() { return numOfVerices(); }
	int E() { return numOfEdges(); }

protected:
	bool allowLoops_;
};

template<typename T>
inline bool Graph<T>::loopsAllowed()
{
	return allowLoops_;
}
