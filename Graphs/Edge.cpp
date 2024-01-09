#include "Edge.h"

Edge::Edge() :
    _vertex1{ 0 },
    _vertex2{ 0 },
    _weight{ 0 }
{}

Edge::Edge(const uint32_t& vertex1, const uint32_t& vertex2, const int32_t& weight) : 
    _vertex1{ vertex1 },
    _vertex2{ vertex2 },
    _weight{ weight }
{}

Edge::Edge(const uint32_t&& vertex1, const uint32_t&& vertex2, const int32_t&& weight) : 
    _vertex1{ vertex1 },
    _vertex2{ vertex2 },
    _weight{ weight }
{}

Edge Edge::reverse() const
{
	return Edge(_vertex2, _vertex1, _weight);
}

std::pair<uint32_t, uint32_t> Edge::verticesGet() const
{
    return std::make_pair(_vertex1, _vertex2);
}

int32_t Edge::weightGet() const
{
    return _weight;
}

std::ostream& operator<<(std::ostream& os, const Edge& e)
{
    os << e.verticesGet().first << " <-(" << e.weightGet() << ")-> " << e.verticesGet().second << '\n';
    return os;
}

bool Edge::operator==(const Edge& edge) const
{
	return _vertex1 == edge.verticesGet().first && _vertex2 == edge.verticesGet().second && _weight == edge.weightGet();
}