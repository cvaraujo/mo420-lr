#include "../inc/graph.h"

void Graph::load_graph(string path) {
    ifstream file;
    file.open(path, fstream::in);
    file >> n >> m;
    
    Edge e;
    edges = vector<Edge>();
    vertices = vector<int>();
    incidenceMatrix = vector<vector<int>>(n, vector<int>());

    for (int i = 0; i < m; i++) {
        file >> e.u >> e.v;
        e.u--, e.v--;
        incidenceMatrix[e.u].push_back(e.v),
        incidenceMatrix[e.v].push_back(e.u);
        if (incidenceMatrix[e.u].size() == 3) vertices.push_back(e.u);
        if (incidenceMatrix[e.v].size() == 3) vertices.push_back(e.v);
        edges.push_back(e);
    }
    file.close();
    cout << "Graph was loaded" << endl;
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
        if (e.u == u && e.v == v) {
            edges[i].weight = weight;
            break;
        }
    }
}