#pragma once
#include "Edge.h"
#include "DisjointSet.h"  
#include <algorithm>
#include <vector>
#include <set>
#include <memory>
#include <numeric>
#include <stack>
#include <deque>
#include <queue>

class Graph {
public:
    Graph();
    Graph(const size_t&);
    Graph(const size_t&&);
    Graph(const size_t&, const std::vector<Edge>&);
    Graph(const size_t&&, const std::vector<Edge>&);
    Graph(const std::vector<std::vector<Edge> >&);
    void edgeAdd(const Edge&);
    void vertexAdd();
    void vertexAdd(const std::vector<Edge>&);
    void print() const;
    void edgeSetMake();
    std::shared_ptr<std::multiset<Edge, edgeComp> > edgeSetGet() const;
    Graph kruskal() const;
    Graph prim() const;
    int32_t costGet() const;
    size_t vertexCountGet() const;
    size_t edgeCountGet() const;
    std::unique_ptr<std::vector<uint32_t> > verticesGet() const;
    std::vector<std::vector<Edge > >& data();
    std::vector<std::vector<Edge > > dataCopy() const;
    int edgeRemove(const Edge&);
    int vertexRemove(const uint32_t&);
    Graph operator=(const Graph&);
private:
    size_t _vertexCount;
    size_t _edgeCount;
    std::vector<std::vector<Edge> > _graph;
    std::shared_ptr<std::multiset<Edge, edgeComp> > _edgeSet;
};
