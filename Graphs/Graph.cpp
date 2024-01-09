#include "Graph.h"

Graph::Graph() : 
    _vertexCount{ 0 },
    _edgeCount{ 0 },
    _edgeSet{ std::make_shared<std::multiset<Edge, edgeComp> >() }
{}

Graph::Graph(const size_t& vertexCount) : 
    _vertexCount{ vertexCount },
    _edgeCount{ 0 },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) },
    _edgeSet{ std::make_shared<std::multiset<Edge, edgeComp> >() }
{}

Graph::Graph(const size_t&& vertexCount) :
    _vertexCount{ vertexCount },
    _edgeCount{ 0 },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) },
    _edgeSet{ std::make_shared<std::multiset<Edge, edgeComp> >() }
{}

Graph::Graph(const size_t& vertexCount, const std::vector<Edge>& edges) : 
    _vertexCount{ vertexCount },
    _edgeCount{ edges.size() },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) },
    _edgeSet{ std::make_shared<std::multiset<Edge, edgeComp> >() }
{
    for (int i = 0; i < _edgeCount; ++i)
    {
        _graph[edges[i].verticesGet().first].push_back(edges[i]);
        _graph[edges[i].verticesGet().second].push_back(edges[i].reverse());
        _edgeSet->insert(edges[i]);
    }
}

Graph::Graph(const size_t&& vertexCount, const std::vector<Edge>& edges) : 
    _vertexCount{ vertexCount },
    _edgeCount{ edges.size() },
    _graph{ std::vector<std::vector<Edge> >(vertexCount) },
    _edgeSet{ std::make_shared<std::multiset<Edge, edgeComp> >() }
{
    for (int i = 0; i < _edgeCount; ++i)
    {
        _graph[edges[i].verticesGet().first].push_back(edges[i]);
        _graph[edges[i].verticesGet().second].push_back(edges[i].reverse());
        _edgeSet->insert(edges[i]);
    }
}

Graph::Graph(const std::vector<std::vector<Edge> >& vertices) :
    _graph{ vertices },
    _vertexCount { vertices.size() },
    _edgeCount{ 0 },
    _edgeSet{ std::make_shared<std::multiset<Edge, edgeComp> >() }
{
    for (const auto& vertex : _graph)
    {
        _edgeCount += vertex.size();
    }
}

void Graph::edgeAdd(const Edge& edge)
{
    uint32_t maxVertex{ std::max(edge.verticesGet().first, edge.verticesGet().second) };
    if (maxVertex >= _graph.size())
    {
		_graph.resize(maxVertex + 1);
		_vertexCount = _graph.size();
	}
    _edgeSet->insert(edge);
    _graph[edge.verticesGet().first].push_back(edge);
    _graph[edge.verticesGet().second].push_back(edge.reverse());
    ++_edgeCount;
}

void Graph::vertexAdd(const std::vector<Edge>& vertexData)
{
    _graph.push_back(vertexData);
	++_vertexCount;
}

void Graph::vertexAdd()
{
    _graph.push_back(std::vector<Edge>());
    ++_vertexCount;
}

void Graph::print() const
{
    std::vector<std::pair<uint32_t, uint32_t> > visited;
    for (const auto& vertex : _graph)
    {
        for (const auto& edge : vertex)
        {
            if (std::find(visited.begin(), visited.end(), edge.verticesGet()) == visited.end()
                &&
                std::find(visited.begin(), visited.end(), std::make_pair(edge.verticesGet().second, edge.verticesGet().first)) == visited.end()
                )
			{
				std::cout << edge;
				visited.push_back(edge.verticesGet());
			}
        }
    }
}

void Graph::edgeSetMake()
{
    _edgeSet = std::make_shared<std::multiset<Edge, edgeComp> >();
    std::vector<std::pair<uint32_t, uint32_t> > visited;
    for (const auto& vertex : _graph)
    {
        for (const auto& edge : vertex)
        {
            if (std::find(visited.begin(), visited.end(), edge.verticesGet()) == visited.end()
                &&
                std::find(visited.begin(), visited.end(), std::make_pair(edge.verticesGet().second, edge.verticesGet().first)) == visited.end()
                )
            {
                visited.push_back(edge.verticesGet());
                _edgeSet->insert(edge);
            }
        }
    }
}

std::shared_ptr<std::multiset<Edge, edgeComp> > Graph::edgeSetGet() const
{
	return _edgeSet;
}

Graph Graph::kruskal() const
{
    DisjointSet<uint32_t> vertexSet;
    vertexSet.setMake(*this->verticesGet());
    Graph minSpanForest(_vertexCount);

    for (const Edge& edge : *_edgeSet)
    {
        if (vertexSet.setFind(edge.verticesGet().first) != vertexSet.setFind(edge.verticesGet().second))
        {
            minSpanForest.edgeAdd(edge);
            vertexSet.setUnion(edge.verticesGet().first, edge.verticesGet().second);
        }
    }
    return minSpanForest;
}

Graph Graph::prim() const
{
    std::priority_queue<Edge, std::vector<Edge>, edgeCompReverse> que;
    std::vector<bool> visited(_vertexCount, false);
    std::vector<int32_t> key(_vertexCount, INT32_MAX);
    std::vector<int32_t> mst(_vertexCount, -1);
    Edge p;
    uint32_t selected{ 0 };
    Graph minSpanForest(_vertexCount);

    key[0] = 0;
    que.push(Edge(0, 0, 0));

    while (!que.empty())
    {
        p = que.top();
        selected = p.verticesGet().second;
        que.pop();
        visited[selected] = true;

        for (const auto& edge : _graph[selected])
        {
            if (!visited[edge.verticesGet().second] && (key[edge.verticesGet().second] > edge.weightGet()))
            {
                key[edge.verticesGet().second] = edge.weightGet();
                que.push(edge);
                mst[edge.verticesGet().second] = selected;
            }
        }
    }

    for (uint32_t i = 1; i < _vertexCount; ++i)
    {
        minSpanForest.edgeAdd(Edge(mst[i], i, key[i]));
    }

    return minSpanForest;
}

int32_t Graph::costGet() const
{
    std::vector<std::pair<uint32_t, uint32_t> > visited;
    int32_t res{ 0 };
    for (const auto& vertex : _graph)
    {
        for (const auto& edge : vertex)
        {
            if (std::find(visited.begin(), visited.end(), edge.verticesGet()) == visited.end()
                &&
                std::find(visited.begin(), visited.end(), std::make_pair(edge.verticesGet().second, edge.verticesGet().first)) == visited.end()
                )
            {
                res += edge.weightGet();
                visited.push_back(edge.verticesGet());
            }
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
    auto res{ std::make_unique<std::vector<uint32_t> >() };
    res->resize(_vertexCount);
    std::iota(res->begin(), res->end(), 0);
    return res;
}

std::vector<std::vector<Edge> >& Graph::data()
{
	return _graph;
}

std::vector<std::vector<Edge> > Graph::dataCopy() const
{
    return _graph;
}

int Graph::edgeRemove(const Edge& edge)
{
    auto& vertex{ _graph[edge.verticesGet().first] };
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

int Graph::vertexRemove(const uint32_t& vertex)
{
    if (vertex >= _graph.size())
    {
		return -1;
	}
	_edgeCount -= _graph[vertex].size();
	_graph[vertex].clear();
    _graph.erase(_graph.begin() + vertex);
    --_vertexCount;
	return 0;
}

Graph Graph::operator=(const Graph& graph)
{
	_vertexCount = graph.vertexCountGet();
	_edgeCount = graph.edgeCountGet();
	_graph = graph.dataCopy();
	return *this;
}