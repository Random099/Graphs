#include "Graph.h"


std::vector< std::set<uint32_t> >::iterator setFind(std::vector< std::set<uint32_t> >& dSet, uint32_t element)
{
    auto it = std::find_if(dSet.begin(), dSet.end(), [element](std::set<uint32_t>& set) { return set.find(element) != set.end(); });
    return it;
}

Graph::Graph() : _vertexCount(0), _edgeCount(0) {}

Graph::Graph(const size_t& vertexCount) : _vertexCount(vertexCount), _edgeCount(0)
{
    _graph = std::vector< std::vector<Edge> >(_vertexCount);
}

Graph::Graph(const size_t&& vertexCount) : _vertexCount(vertexCount), _edgeCount(0)
{
    _graph = std::vector< std::vector<Edge> >(_vertexCount);
}

Graph::Graph(const size_t& vertexCount, const std::vector<Edge>& edges) : _vertexCount(vertexCount), _edgeCount(edges.size())
{
    _graph = std::vector< std::vector<Edge> >(_vertexCount);
    for (int i = 0; i < _edgeCount; ++i)
    {
        _graph[edges[i].verticesGet().first].push_back(edges[i]);
    }
}

Graph::Graph(const size_t&& vertexCount, const std::vector<Edge>& edges) : _vertexCount(vertexCount), _edgeCount(edges.size())
{
    _graph = std::vector< std::vector<Edge> >(_vertexCount);
    for (int i = 0; i < _edgeCount; ++i)
    {
        _graph[edges[i].verticesGet().first].push_back(edges[i]);
    }
}

Graph::Graph(const std::vector< std::vector<Edge> >& vertices) : _graph(vertices)
{
    _vertexCount = _graph.size();
    _edgeCount = 0;
    for (const auto& vertex : _graph)
    {
        _edgeCount += vertex.size();
    }
}

void Graph::edgeAdd(const Edge & edge)
{
    _graph[edge.verticesGet().first].push_back(edge);
    ++_edgeCount;
}

void Graph::print() const
{
    for (const auto& vertex : _graph)
    {
        for (const auto& edge : vertex)
        {
            std::cout << edge;
        }
    }
}

std::unique_ptr< std::multiset<Edge, edgeComp> > Graph::edgeSetGet() const
{
    auto res = std::make_unique< std::multiset<Edge, edgeComp> >();
    for (const auto& vertex : _graph)
    {
        for (const auto& edge : vertex)
        {
            res->insert(edge);
        }
    }
    return res;
}

Graph Graph::kruskal() const
{
    std::unique_ptr< std::multiset<Edge, edgeComp> > edges = this->edgeSetGet();
    DisjointSet<uint32_t> vertexSet;
    vertexSet.setMake(*this->verticesGet());
    Graph minSpanForest(_vertexCount);

    for (const Edge& edge : *edges)
    {
        if (vertexSet.setFind(edge.verticesGet().first) != vertexSet.setFind(edge.verticesGet().second))
        {
            minSpanForest.edgeAdd(edge);
            vertexSet.setUnion(edge.verticesGet().first, edge.verticesGet().second);
        }
    }
    return minSpanForest;
}

int32_t Graph::costGet() const
{
    int32_t res = 0;
    for (const auto& vertex : _graph)
    {
        for (const auto& edge : vertex)
        {
            res += edge.weightGet();
        }
    }
    return res;
}

size_t Graph::vertexCountGet() const
{
    return _vertexCount;
}

std::unique_ptr< std::vector<uint32_t> > Graph::verticesGet() const
{
    auto res = std::make_unique< std::vector<uint32_t> >();
    res->resize(_vertexCount);
    std::iota(res->begin(), res->end(), 0);
    return res;
}