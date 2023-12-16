#pragma once
#include "Edge.h"
#include <algorithm>
#include <vector>
#include <set>
#include <memory>

std::vector< std::set<uint32_t> >::iterator setFind(std::vector< std::set<uint32_t> >&, uint32_t);

class Graph {
public:
    Graph();
    Graph(const size_t&);
    Graph(const size_t&&);
    Graph(const size_t&, const std::vector<Edge>&);
    Graph(const size_t&&, const std::vector<Edge>&);
    Graph(const std::vector< std::vector<Edge> >& vertices);
    void addEdge(const Edge& edge);
    void print() const;
    std::unique_ptr< std::multiset<Edge, edgeComp> > edgeSetGet() const;
    Graph kruskal() const;
    int32_t costGet() const;
    size_t vertexCountGet() const;
private:
    size_t _vertexCount;
    size_t _edgeCount;
    std::vector< std::vector<Edge> > _vertices;
};
