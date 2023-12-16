#pragma once
#include <cstdint>
#include <utility>
#include <iostream>

class Edge {
public:
    Edge(const uint32_t&, const uint32_t&, const int32_t&);
    Edge(const uint32_t&&, const uint32_t&&, const int32_t&&);
    Edge();
    std::pair<const uint32_t, const uint32_t> getVertices() const;
    const int& getWeight() const;
    friend std::ostream& operator<<(std::ostream& os, const Edge& e);
private:
    uint32_t _vertex1;
    uint32_t _vertex2;
    int32_t _weight;
};

struct edgeComp
{
    bool operator()(const Edge& edge1, const Edge& edge2) const
    {
        return edge1.getWeight() < edge2.getWeight();
    }
};