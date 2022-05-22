#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int size) : aMatrix(size)
{
    for (std::vector<unsigned char> &row : aMatrix)
    {
        row.resize(size, 0);
    }
}

MatrixGraph::MatrixGraph(const IGraph &graph) : aMatrix(graph.VerticesCount())
{
    for (std::vector<unsigned char> &row : aMatrix)
    {
        row.resize(graph.VerticesCount(), 0);
    }

    for (int from = 0; from < graph.VerticesCount(); from++)
    {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices)
        {
            aMatrix[from][to] += 1;
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) 
{
    assert(from >= 0 && from <= aMatrix.size());
    assert(to >= 0 && to <= aMatrix.size());
    aMatrix[from][to] += 1;
}

int MatrixGraph::VerticesCount() const 
{
    return (int)aMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const 
{
    assert(vertex >= 0 && vertex < aMatrix.size());
    std::vector<int> nextVertices;
    for (int to = 0; to < aMatrix.size(); to++)
    {
        if (aMatrix[vertex][to] != 0)
        {
            nextVertices.push_back(to);
        }
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < aMatrix.size());
    std::vector<int> prevVertices;
    for (int from = 0; from < aMatrix.size(); from++)
    {
        if (aMatrix[from][vertex] != 0)
        {
            prevVertices.push_back(from);
        }
    }
    return prevVertices;
}