#include "../inc/graph.h"

void Graph::load_graph(string path) {
    ifstream file;
    file.open(path, fstream::in);
    file >> n >> m;
    Edge e;
    edges = vector<Edge>();
    for (int i = 0; i < m; i++) {
        file >> e.u >> e.v;
        e.u--, e.v--;
        edges.push_back(e);
    }
    file.close();
}

void Graph::print_graph() {
    for (auto e : edges) {
        cout << e.u << " - " << e.v << " = " << e.weight << endl;
    }
}

void Graph::set_edge_value(int u, int v, double weight){
    Edge e;
    for (int i = 0; i < m; i++){
        e = edges[i];
        if (((e.u == u) && (e.v == v)) || ((e.u == v) && (e.v == u))) {
            edges[i].weight = weight;
            break;
        }
    }
}