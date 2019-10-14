#include "../inc/graph.h"
#include "../inc/model.h"

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        Graph *graph = new Graph();
        graph->load_graph("instances_tests/inputs/Spd_RF2_200_222_3811.txt");//"Spd_RF2_20_27_219.txt");//"toy.txt");
        // graph->print_graph();
        auto *model = new Model(graph);
        model->lagrangean();
        return 0;
    } else {
//         auto *data = new Data(argv[1]);
//         auto *model = new Model(data);
//         clock_t t0 = clock();
//         model->lagrangean();
//         double endTime = double(clock() - t0) / double(CLOCKS_PER_SEC);
// //        model->showSolution(argv[1], "results_lagrangean_SHP.txt", atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), endTime);
    }

    return 0;
}

// #include "../inc/kruskal.h"

// void salve_solution(Solution solution, string path) {
//     //cout << "Salvando Solution..." << endl;
//     ofstream file;
//     file.open(path);
//     file << solution.value << endl;
//     Edge e;
//     for (int i=0; i<(int)solution.edges.size(); i++) {
//         e = solution.edges[i];
//         file << e.u << ' ' << e.v << ' ' << e.weight << endl;
//     }
//     if(solution.value > 0) 
//         cout << "The solution has saved" << endl;
//     file.close();
// }

// int main(int argc, char const *argv[]){
//     if (argc < 2) {
//         printf("Input Error!");
//         return 0;
//     }

//     Graph graph;
//     graph.load_graph(argv[1]);

//     MST_Kruskal mst(graph);

//     Solution solution;
//     solution = mst.solve();
//     cout << "Tree weight: " << solution.value << endl;
//     cout << "Edges:" << endl;
//     for (auto e : solution.edges) {
//         cout << e.u << " -- " << e.v << endl;
//     }
//     return 0;
// }
