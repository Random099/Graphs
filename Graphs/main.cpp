#include "main.h"

struct disjointSetComp
{
    bool operator()(const std::set<uint32_t>& set1, const std::set<uint32_t>& set2) const
    {
        return *set1.rbegin() < *set2.rbegin();
    }
};

int main()
{
    Graph graphFirst(4);
    graphFirst.addEdge(Edge(0, 1, 1));
    graphFirst.addEdge(Edge(0, 2, 3));
    graphFirst.addEdge(Edge(0, 3, 4));
    graphFirst.addEdge(Edge(1, 2, 2));
    graphFirst.addEdge(Edge(2, 3, 5));
    std::cout << "Graph 1:\n";
    graphFirst.print();
    std::cout << "Cost: " << graphFirst.costGet() << '\n';

    std::cout << "Min Span Forest:\n";
    Graph graphFirstMinSpanForest = graphFirst.kruskal();
    graphFirstMinSpanForest.print();
    std::cout << "Cost: " << graphFirstMinSpanForest.costGet() << '\n';
    
    
    Graph graphSecond(7);
    graphSecond.addEdge(Edge(0, 1, 7));
    graphSecond.addEdge(Edge(0, 3, 5));
    graphSecond.addEdge(Edge(1, 2, 8));
    graphSecond.addEdge(Edge(1, 3, 9));
    graphSecond.addEdge(Edge(1, 4, 7));
    graphSecond.addEdge(Edge(2, 4, 5));
    graphSecond.addEdge(Edge(3, 4, 15));
    graphSecond.addEdge(Edge(3, 5, 6));
    graphSecond.addEdge(Edge(4, 5, 8));
    graphSecond.addEdge(Edge(4, 6, 9));
    graphSecond.addEdge(Edge(5, 6, 11));
    std::cout << "Graph 2:\n";
    graphSecond.print();
    std::cout << "Cost: " << graphSecond.costGet() << '\n';

    std::cout << "Min Span Forest:\n";
    Graph graphSecondMinSpanForest = graphSecond.kruskal();
    graphSecondMinSpanForest.print();
    std::cout << "Cost: " << graphSecondMinSpanForest.costGet() << '\n';
    
    return 0;
}
