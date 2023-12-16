#include "Edge.h"

Edge::Edge(const uint32_t& vertex1, const uint32_t& vertex2, const int32_t& weight) : _vertex1(vertex1), _vertex2(vertex2), _weight(weight) {}

Edge::Edge(const uint32_t&& vertex1, const uint32_t&& vertex2, const int32_t&& weight) : _vertex1(vertex1), _vertex2(vertex2), _weight(weight) {}

Edge::Edge() : _vertex1(0), _vertex2(0), _weight(0) {}

std::pair<const uint32_t, const uint32_t> Edge::getVertices() const
{
    return std::make_pair(_vertex1, _vertex2);
}

const int& Edge::getWeight() const
{
    return _weight;
}

std::ostream& operator<<(std::ostream& os, const Edge& e)
{
    os << e.getVertices().first << " <-(" << e.getWeight() << ")-> " << e.getVertices().second << '\n';
    return os;
}