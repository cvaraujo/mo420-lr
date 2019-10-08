#include "graph.h"

void Graph::load_graph(string path) {
    ifstream file;
    file.open(path, fstream::in);
    file >> n >> m;
    Edge e, aux;
    adj.resize(n);
    for (int i = 0; i < m; i++) {
        file >> e.u >> e.v >> e.weight;
        adj[e.u].push_back(e);
        aux.u = e.v;
        aux.v = e.u;
        aux.weight = e.weight;
        adj[e.v].push_back(aux);
        edges.push_back(e);
    }

    //cout << "Arquivo Carregado" << endl;
    file.close();
}

void Graph::print_graph() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < int(adj[i].size()); j++) {
            Edge e = adj[i][j];
            cout << '(' << e.u << ',' << e.v << ") - " << e.weight << endl;
        }
    }
}
