#include <iostream>
#include <functional>
#include <vector>
#include <iterator>
#include <queue>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"



void BFC(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;

    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();
        func(currentVertex);
        for (int nextVertex : graph.GetNextVertices
        (currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFC(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
        {
            BFC(graph, i, visited, func);
        }
    }

}

void DFC(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);

    for (int nextVertex : graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            DFC(graph, nextVertex, visited, func);
        }
    }
}

void mainDFC(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
        {
            DFC(graph, i, visited, func);
        }
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;

    for (int nextVertex : graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            topologicalSortInternal(graph, nextVertex, visited, sorted);
        }
    }
    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        if (!visited[i])
        {
            topologicalSortInternal(graph, i, visited, sorted);
        }
    }
    return sorted; 
}

void showGraph(const IGraph &graph)
{
    // VerticesCount + GetNextVertices
    mainBFC(graph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    // GetPrevVertices(4)
    std::vector<int> prevVertices = graph.GetPrevVertices(4);
    std::copy(prevVertices.begin(), prevVertices.end(), std::ostream_iterator<int>(std::cout, " "));
}

int main(int argc, const char *argv[])
{
    ListGraph listGraph(7);

    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);

    std::cout << "\nListGraph" << std::endl;
    showGraph(listGraph);

    std::cout << "\nMatrixGraph" << std::endl;

    MatrixGraph matrixGraph(listGraph);

    showGraph(matrixGraph);

    std::cout << "\nSetGraph" << std::endl;

    SetGraph setGraph(matrixGraph);

    showGraph(setGraph);

    std::cout << "\nArcGraph" << std::endl;

    ArcGraph arcGraph(setGraph);

    showGraph(arcGraph);
}