#include "SetGraph.h"


SetGraph::SetGraph(int size) : aSets(size) {}
SetGraph::SetGraph(const IGraph &graph) : aSets(graph.VerticesCount())
{
    for (int from = 0; from < graph.VerticesCount(); from++)
    {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices)
        {
            aSets[from].insert(to);
        }
    }
}


void SetGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && from <= aSets.size());
    assert(to >= 0 && to <= aSets.size());
    aSets[from].insert(to);
}

int SetGraph::VerticesCount() const 
{
    return (int)aSets.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < aSets.size());
    std::vector<int> nextVertices(aSets[vertex].size());
    std::copy(aSets[vertex].begin(), aSets[vertex].end(), nextVertices.begin());
    return nextVertices;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const 
{
    assert(vertex >= 0 && vertex < aSets.size());
    std::vector<int> prevVertices;

    for (int from = 0; from < aSets.size(); from++)
    {
        for (int to: aSets[from])
        {
            if (vertex == to)
            {
                prevVertices.push_back(from);
            }
        }
    }
    return prevVertices;
}