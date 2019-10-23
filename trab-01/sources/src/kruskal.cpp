#include "../inc/kruskal.h"
#include "../inc/union_find.h"

MST_Kruskal::MST_Kruskal(Graph graph) {
    this->graph = graph;
}

Solution MST_Kruskal::solve() {
    UnionFind set(graph.n); 

    for (int i = 0; i < graph.n; i++)
        set.make_set(i);

    sort(graph.edges.begin(), graph.edges.end());

    Solution solution;
    // for (auto e : graph.bridges){
    //     if (!set.same_set_with_find_path_compression(e.u, e.v)) {
    //         set.union_by_rank_with_path_compression(e.u, e.v);
    //         solution.value += e.weight;
    //         solution.edges.push_back(e);
    //     }
    // }

    for (int i = 0; i < graph.m; i++) {
        Edge e = graph.edges[i];
        // if (!graph.isBridge[e.u][e.v]) {
            if (!set.same_set_with_find_path_compression(e.u, e.v)) {
                set.union_by_rank_with_path_compression(e.u, e.v);
                solution.value += e.weight;
                solution.edges.push_back(e);
            }
        // }
    }
    return solution;
}

Solution MST_Kruskal::solve(vector<Edge> edges, UnionFind *set) {
    vector<Edge> edges_return = vector<Edge>(edges);
    sort(edges_return.begin(), edges_return.end());
    Solution solution;

    for (auto e : edges_return) {
        if (!set->same_set_with_find_path_compression(e.u, e.v)) {
            set->union_by_rank_with_path_compression(e.u, e.v);
            solution.edges.push_back(e);
            solution.value += e.weight;
        }
    }

    return solution;
}