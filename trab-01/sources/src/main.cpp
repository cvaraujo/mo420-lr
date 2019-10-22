#include "../inc/graph.h"
#include "../inc/model.h"

int main(int argc, const char *argv[]) {
    if (argc >= 4) {
        srand(time(NULL));
        Graph *graph = new Graph();
        graph->load_graph(argv[1]);
        graph->bridge();
        auto *model = new Model(graph);
        model->lagrangean(atoi(argv[3]));
        model->showSolution(argv[2]);
    } else cout << "Wrong number of parameters" << endl;
    return 0;
}
