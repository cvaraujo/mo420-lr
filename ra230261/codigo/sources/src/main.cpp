#include "../inc/graph.h"
#include "../inc/subgradient.h"

int main(int argc, const char *argv[]) {
    if (argc >= 4) {
        srand(time(NULL));
        Graph *graph = new Graph();
        graph->load_graph(argv[1]);
        graph->bridge();
        auto *subgradient = new Subgradient(graph);
        subgradient->lagrangean(atoi(argv[3]));
        subgradient->showSolution(argv[2]);
    } else cout << "Wrong number of parameters" << endl;
    return 0;
}
