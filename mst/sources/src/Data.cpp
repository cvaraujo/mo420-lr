//
// Created by carlos on 05/03/19.
//

#include "../headers/Data.h"

Data::Data(const char *instance) {
    int u, v;
    string token;
    ifstream fileGraph;
    edges = vector<Edge *>();

    fileGraph.open(instance, fstream::in);
    fileGraph >> n >> m;
    while (!fileGraph.eof()) {
        fileGraph >> u >> v;
        Edge *edge = new Edge(u, v);
        edges.push_back(edge);
    }
}


void Data::showData() {
    cout << "Edges: " << endl;
    for (auto *edge : edges) {
        cout << edge->getO() << " " << edge->getD() << endl;
    }

}