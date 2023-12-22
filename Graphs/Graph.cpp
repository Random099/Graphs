#include "Graph.h"

Graph::Graph() : 
    _vertexCount{ 0 },
    _edgeCount{ 0 }
{}

Graph::Graph(const size_t& vertexCount) : 
    _vertexCount{ vertexCount },
    _edgeCount{ 0 },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) }
{}

Graph::Graph(const size_t&& vertexCount) :
    _vertexCount{ vertexCount },
    _edgeCount{ 0 },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) }
{}

Graph::Graph(const size_t& vertexCount, const std::vector<Edge>& edges) : 
    _vertexCount{ vertexCount },
    _edgeCount{ edges.size() },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) }
{
    for (int i = 0; i < _edgeCount; ++i)
    {
        _graph[edges[i].verticesGet().first].push_back(edges[i]);
    }
}

Graph::Graph(const size_t&& vertexCount, const std::vector<Edge>& edges) : 
    _vertexCount{ vertexCount },
    _edgeCount{ edges.size() },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) }
{
    for (int i = 0; i < _edgeCount; ++i)
    {
        _graph[edges[i].verticesGet().first].push_back(edges[i]);
    }
}

Graph::Graph(const std::vector<std::vector<Edge> >& vertices) :
    _graph{ vertices },
    _vertexCount { vertices.size() },
    _edgeCount{ 0 }
{
    for (const auto& vertex : _graph)
    {
        _edgeCount += vertex.size();
    }
}

void Graph::edgeAdd(const Edge& edge)
{
    uint32_t maxVertex = std::max(edge.verticesGet().first, edge.verticesGet().second);
    if (maxVertex >= _graph.size())
    {
		_graph.resize(maxVertex + 1);
		_vertexCount = _graph.size();
	}
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

std::unique_ptr<std::multiset<Edge, edgeComp> > Graph::edgeSetGet() const
{
    auto res = std::make_unique<std::multiset<Edge, edgeComp> >();
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
    std::unique_ptr<std::multiset<Edge, edgeComp> > edges = this->edgeSetGet();
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

size_t Graph::edgeCountGet() const
{
	return _edgeCount;
}

std::unique_ptr<std::vector<uint32_t> > Graph::verticesGet() const
{
    auto res = std::make_unique<std::vector<uint32_t> >();
    res->resize(_vertexCount);
    std::iota(res->begin(), res->end(), 0);
    return res;
}

std::vector<std::vector<Edge> >& Graph::_data()
{
	return _graph;
}

int Graph::removeEdge(const Edge& edge)
{
    auto& vertex = _graph[edge.verticesGet().first];
	auto it = std::find_if(vertex.begin(), vertex.end(),
        [&](const Edge& e) -> bool
        {
			return e.verticesGet().first == edge.verticesGet().first && e.verticesGet().second == edge.verticesGet().second;
		}
    	);
    if (it != vertex.end())
    {
		vertex.erase(it);
		--_edgeCount;
		return 0;
	}
	return -1;
}

int Graph::removeVertex(const uint32_t& vertex)
{
    if (vertex >= _graph.size())
    {
		return -1;
	}
	_edgeCount -= _graph[vertex].size();
	_graph[vertex].clear();
	return 0;
}