#ifndef GRAPH_H
#define GRAPH_H

#include "include.h"

using namespace std;

class Edge {
public:
    int u;
    int v;
    double weight = 0;

    Edge() {}

    Edge(int u, int v, double weight) {
        this->u = u;
        this->v = v;
        this->weight = weight;
    }

    bool operator<(const Edge &Edge) const {
        return (this->weight < Edge.weight);
    }
};

class Graph {
public:
    int n, m;
    vector<Edge> edges;
    vector<int> vertices;
    vector<vector<int>> incidenceMatrix;

    Graph() {
    }

    void load_graph(string path);

    void print_graph();

    void set_edge_value(int u, int v, double weight);
};

#endif
