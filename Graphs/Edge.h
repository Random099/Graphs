#pragma once
#include <cstdint>
#include <utility>
#include <iostream>

class Edge {
public:
    Edge();
    Edge(const uint32_t&, const uint32_t&, const int32_t&);
    Edge(const uint32_t&&, const uint32_t&&, const int32_t&&);
    std::pair<uint32_t, uint32_t> verticesGet() const;
    const int& weightGet() const;
    friend std::ostream& operator<<(std::ostream& os, const Edge& e);
    bool operator==(const Edge& edge) const;
private:
    uint32_t _vertex1;
    uint32_t _vertex2;
    int32_t _weight;
};

struct edgeComp
{
    bool operator()(const Edge& edge1, const Edge& edge2) const
    {
        return edge1.weightGet() < edge2.weightGet();
    }
};