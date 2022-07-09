#include "ListGraph.h"


ListGraph::ListGraph(int size) : aLists(size) {}
ListGraph::ListGraph(const IGraph &graph) : aLists(graph.VerticesCount())
{
    for (int from = 0; from < graph.VerticesCount(); from++)
    {
        aLists.push_back(graph.GetNextVertices(from));
    }
}


void ListGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && from <= aLists.size());
    assert(to >= 0 && to <= aLists.size());
    aLists[from].push_back(to);
}

int ListGraph::VerticesCount() const 
{
    return (int)aLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < aLists.size());
    return aLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const 
{
    assert(vertex >= 0 && vertex < aLists.size());
    std::vector<int> prevVertices;

    for (int from = 0; from < aLists.size(); from++)
    {
        for (int to: aLists[from])
        {
            if (vertex == to)
            {
                prevVertices.push_back(from);
            }
        }
    }
    return prevVertices;
}