#include "Graph.h"


std::vector< std::set<uint32_t> >::iterator setFind(std::vector< std::set<uint32_t> >& dSet, uint32_t element)
{
    auto it = std::find_if(dSet.begin(), dSet.end(), [element](std::set<uint32_t>& set) mutable { return set.find(element) != set.end(); });
    return it;
}

Graph::Graph() : _vertexCount(0), _edgeCount(0) {}

Graph::Graph(const size_t& vertexCount) : _vertexCount(vertexCount), _edgeCount(0)
{
    _vertices = std::vector< std::vector<Edge> >(_vertexCount);
}

Graph::Graph(const size_t&& vertexCount) : _vertexCount(vertexCount), _edgeCount(0)
{
    _vertices = std::vector< std::vector<Edge> >(_vertexCount);
}

Graph::Graph(const size_t& vertexCount, const std::vector<Edge>& edges) : _vertexCount(vertexCount), _edgeCount(edges.size())
{
    _vertices = std::vector< std::vector<Edge> >(_vertexCount);
    for (int i = 0; i < _edgeCount; ++i)
    {
        _vertices[edges[i].getVertices().first].push_back(edges[i]);
    }
}

Graph::Graph(const size_t&& vertexCount, const std::vector<Edge>& edges) : _vertexCount(vertexCount), _edgeCount(edges.size())
{
    _vertices = std::vector< std::vector<Edge> >(_vertexCount);
    for (int i = 0; i < _edgeCount; ++i)
    {
        _vertices[edges[i].getVertices().first].push_back(edges[i]);
    }
}

Graph::Graph(const std::vector< std::vector<Edge> >& vertices) : _vertices(vertices)
{
    _vertexCount = _vertices.size();
    _edgeCount = 0;
    for (const auto& vertex : _vertices)
    {
        _edgeCount += vertex.size();
    }
}

void Graph::addEdge(const Edge & edge)
{
    _vertices[edge.getVertices().first].push_back(edge);
    ++_edgeCount;
}

void Graph::print() const
{
    for (const auto& vertex : _vertices)
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
    for (const auto& vertex : _vertices)
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
    std::vector< std::set<uint32_t>> vertexList;
    for (uint32_t i = 0; i < _vertexCount; ++i)
    {
        vertexList.push_back(std::set<uint32_t>{ i });
    }
    Graph minSpanForest(_vertexCount);

    for (const Edge& edge : *edges)
    {
        auto vertexFirstIt = setFind(vertexList, edge.getVertices().first);
        auto vertexSecondIt = setFind(vertexList, edge.getVertices().second);

        if (*vertexFirstIt->rbegin() != *vertexSecondIt->rbegin())
        {
            minSpanForest.addEdge(edge);
            vertexFirstIt->insert(vertexSecondIt->begin(), vertexSecondIt->end());
            vertexList.erase(vertexSecondIt);
        }
    }
    return minSpanForest;
}

int32_t Graph::costGet() const
{
    int32_t res = 0;
    for (const auto& vertex : _vertices)
    {
        for (const auto& edge : vertex)
        {
            res += edge.getWeight();
        }
    }
    return res;
}

size_t Graph::vertexCountGet() const
{
    return _vertexCount;
}