#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Edge {
public:
    int u;
    int v;
    double weight;

    Edge() {}

    Edge(int u, int v, int weight) {
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
    int n;
    int m;
    vector<vector<Edge>> adj;
    vector<Edge> edges;

    Graph() {
    }

    void load_graph(string path);

    void print_graph();
};

#endif
