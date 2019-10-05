#include "../inc/kruskal.h"

void salve_solution(Solution solution, string path) {
    //cout << "Salvando Solution..." << endl;
    ofstream file;
    file.open(path);
    file << solution.value << endl;
    Edge e;
    for (int i=0; i<(int)solution.edges.size(); i++) {
        e = solution.edges[i];
        file << e.u << ' ' << e.v << ' ' << e.weight << endl;
    }
    if(solution.value > 0) 
        cout << "The solution has saved" << endl;
    file.close();
}

int main(int argc, char const *argv[]){
    if (argc < 2) {
        printf("Input Error!");
        return 0;
    }

    Graph graph;
    graph.load_graph(argv[1]);

    MST_Kruskal mst(graph);

    Solution solution;
    solution = mst.solve();
    cout << "Tree weight: " << solution.value << endl;
    cout << "Edges:" << endl;
    for (auto e : solution.edges) {
        cout << e.u << " -- " << e.v << endl;
    }
    return 0;
}
