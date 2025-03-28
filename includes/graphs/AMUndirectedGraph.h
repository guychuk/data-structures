#pragma once

#include "UndirectedGraph.h"
#include <stdexcept>

class AMUndirectedGraph : public UndirectedGraph<int>
{
public:
	AMUndirectedGraph(int n, bool allowLoops = false);
	~AMUndirectedGraph();

	// from Graph class

	virtual int numOfVerices();
	virtual int numOfEdges();
	virtual bool hasVertex(int vertex);
	virtual bool hasEdge(int vertex1, int vertex2);
	virtual bool hasEulerianCycle();
	virtual void addEdge(int vertex1, int vertex2);
	virtual void removeEdge(int vertex1, int vertex2);

	// degrees

	virtual int degreeOf(int vertex);
	virtual int maxDegree();
	virtual int minDegree();

	// conncetivity

	virtual bool isConnceted();

private:
	int n_;
	bool** matrix_;
	int e_;
	int* degrees_;
	int maxDegree_;
	int minDegree_;

	void DFS(int prev, int curr, int* visitsArr);
};

AMUndirectedGraph::AMUndirectedGraph(int n, bool allowLoops)
{
	if (n < 0)
	{
		throw std::invalid_argument("a graph cannot have a negative number of vertices");
	}

	n_ = n;
	matrix_ = new bool*[n];

	for (int i = 0; i < n; i++)
	{
		matrix_[i] = new bool[n];

		for (int j = 0; j < n; j++)
		{
			matrix_[i][j] = false;
		}
	}

	e_ = 0;

	degrees_ = new int[n];

	for (int i = 0; i < n; i++)
	{
		degrees_[i] = 0;
	}

	maxDegree_ = 0;
	minDegree_ = 0;

	allowLoops_ = allowLoops;
}

AMUndirectedGraph::~AMUndirectedGraph()
{
	for (int i = 0; i < n_; i++)
	{
		delete[] matrix_[i];
	}

	delete[] matrix_;
	delete[] degrees_;
	n_ = -1;
	e_ = -1;
	maxDegree_ = -1;
	minDegree_ = -1;
	matrix_ = nullptr;
	degrees_ = nullptr;
}

int AMUndirectedGraph::numOfVerices()
{
	return n_;
}

int AMUndirectedGraph::numOfEdges()
{
	return e_;
}

bool AMUndirectedGraph::hasVertex(int vertex)
{
	return 0 <= vertex && vertex < n_;
}

bool AMUndirectedGraph::hasEdge(int vertex1, int vertex2)
{
	if (!hasVertex(vertex1) || !hasVertex(vertex2))
	{
		throw std::invalid_argument("vertex not found");
	}

	return matrix_[vertex1][vertex2];
}

bool AMUndirectedGraph::hasEulerianCycle()
{
	if (!isConnceted())
	{
		return false;
	}

	for (int i = 0; i < n_; i++)
	{
		if (degrees_[i] % 2)
		{
			return false;
		}
	}

	return true;
}

void AMUndirectedGraph::addEdge(int vertex1, int vertex2)
{
	if (!hasEdge(vertex1, vertex2) && ((vertex1 != vertex2) || allowLoops_))
	{
		matrix_[vertex1][vertex2] = true;
		matrix_[vertex2][vertex1] = true;
		degrees_[vertex1]++;
		degrees_[vertex2]++;
		e_++;

		if (maxDegree_ < degrees_[vertex1])
		{
			maxDegree_ = degrees_[vertex1];
		}
		else if (maxDegree_ < degrees_[vertex2])
		{
			maxDegree_ = degrees_[vertex2];
		}
	}
}

void AMUndirectedGraph::removeEdge(int vertex1, int vertex2)
{
	if (hasEdge(vertex1, vertex2))
	{
		matrix_[vertex1][vertex2] = false;
		matrix_[vertex2][vertex1] = false;
		degrees_[vertex1]--;
		degrees_[vertex2]--;
		e_--;

		if (minDegree_ > degrees_[vertex1])
		{
			minDegree_ = degrees_[vertex1];
		}
		else if (minDegree_ > degrees_[vertex2])
		{
			minDegree_ = degrees_[vertex2];
		}
	}
}

int AMUndirectedGraph::degreeOf(int vertex)
{
	if (!hasVertex(vertex))
	{
		throw std::invalid_argument("vertex not found");
	}

	return degrees_[vertex];
}

int AMUndirectedGraph::maxDegree()
{
	return maxDegree_;
}

int AMUndirectedGraph::minDegree()
{
	return minDegree_;
}

bool AMUndirectedGraph::isConnceted()
{
	if (e_ < n_ - 1)
	{
		return false;
	}

	int* visits = new int[n_];

	for (int i = 0; i < n_; i++)
	{
		visits[i] = -1;
	}

	DFS(0, 0, visits);

	for (int i = 0; i < n_; i++)
	{
		if (visits[i] == -1)
		{
			delete[] visits;
			return false;
		}
	}

	delete[] visits;
	return true;
}

void AMUndirectedGraph::DFS(int prev, int curr, int* visitsArr)
{
	visitsArr[curr] = prev;

	for (int i = 0; i < n_; i++)
	{
		if (matrix_[curr][i] && visitsArr[i] == -1)
		{
			DFS(curr, i, visitsArr);
		}
	}
}
