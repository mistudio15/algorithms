#include <set>
#include "ArcGraph.h"

ArcGraph::ArcGraph(int size) : count(size){}

ArcGraph::ArcGraph(const IGraph &graph) : count(graph.VerticesCount())
{
    for (int from = 0; from < graph.VerticesCount(); from++)
    {
        std::vector<int> nextVertices = graph.GetNextVertices(from);
        for (int to : nextVertices)
        {
            aArcs.push_back(std::make_pair(from, to));
            // aArcs.push_back(std::pair<int, int>(from, to));
        }
    }
}


void ArcGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && from <= count);
    assert(to >= 0 && to <= count);
    aArcs.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const 
{
    return count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < count);
    std::vector<int> nextVertices;
    for (auto &pair : aArcs)
    {
        if (pair.first == vertex)
        {
            nextVertices.push_back(pair.second);
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const 
{
    assert(vertex >= 0 && vertex < count);
    std::vector<int> prevVertices;
    for (auto &pair : aArcs)
    {
        if (pair.second == vertex)
        {
            prevVertices.push_back(pair.first);
        }
    }
    return prevVertices;
}