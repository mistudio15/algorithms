#include <iostream>
#include <functional>
#include <vector>
#include <iterator>
#include <queue>
#include <limits>

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

int FindRoute(const IGraph &graph, int s, int t)
{
    std::vector<int> routes(graph.VerticesCount(), std::numeric_limits<int>::max() - 1);
    std::vector<int> k(graph.VerticesCount(), 0);
    k[s] = 1;
    std::queue<int> q;
    q.push(s);
    routes[s] = 0;
    while (!q.empty())
    {
        int currentVertex = q.front();
        q.pop();
        for (int nextVertex : graph.GetNextVertices(currentVertex))
        {
            if (routes[nextVertex] > routes[currentVertex] + 1)
            {
                std::cout << ">" << currentVertex << " " << k[currentVertex] << "|" << nextVertex << " " << k[nextVertex] << std::endl;
                routes[nextVertex] = routes[currentVertex] + 1;
                k[nextVertex] = k[currentVertex];
                q.push(nextVertex);
            }
            else if (routes[nextVertex] == routes[currentVertex] + 1)
            {
                std::cout << "=" << currentVertex << " " << k[currentVertex] << "|" << nextVertex <<" " <<  k[nextVertex] << std::endl;
                k[nextVertex] += k[currentVertex];
            }

        }
    }
    // std::copy(k.begin(), k.end(), std::ostream_iterator<int>(std::cout, " "));
    return k[t];
}

void showGraph(const IGraph &graph)
{
    // VerticesCount + GetNextVertices
    mainBFC(graph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    // GetPrevVertices(4)
    std::vector<int> prevVertices = graph.GetPrevVertices(3);
    std::copy(prevVertices.begin(), prevVertices.end(), std::ostream_iterator<int>(std::cout, " "));
}

int main(int argc, const char *argv[])
{
    ListGraph listGraph(4);

    // listGraph.AddEdge(0, 1);
    // listGraph.AddEdge(0, 5);
    // listGraph.AddEdge(1, 2);
    // listGraph.AddEdge(1, 3);
    // listGraph.AddEdge(1, 5);
    // listGraph.AddEdge(1, 6);
    // listGraph.AddEdge(3, 2);
    // listGraph.AddEdge(3, 4);
    // listGraph.AddEdge(3, 6);
    // listGraph.AddEdge(5, 4);
    // listGraph.AddEdge(5, 6);
    // listGraph.AddEdge(6, 4);

    // listGraph.AddEdge(1, 0);
    // listGraph.AddEdge(5, 0);
    // listGraph.AddEdge(2, 1);
    // listGraph.AddEdge(3, 1);
    // listGraph.AddEdge(5, 1);
    // listGraph.AddEdge(6, 1);
    // listGraph.AddEdge(2, 3);
    // listGraph.AddEdge(4, 3);
    // listGraph.AddEdge(6, 3);
    // listGraph.AddEdge(4, 5);
    // listGraph.AddEdge(6, 5);
    // listGraph.AddEdge(4, 6);

    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 2);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(2, 3);

    listGraph.AddEdge(1, 0);
    listGraph.AddEdge(2, 0);
    listGraph.AddEdge(2, 1);
    listGraph.AddEdge(3, 1);
    listGraph.AddEdge(3, 2);

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

    std::cout << std::endl;

    std::cout << "k = " << FindRoute(arcGraph, 0, 3) << std::endl;
}