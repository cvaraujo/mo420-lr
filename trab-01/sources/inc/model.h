//
// Created by carlos on 06/03/19.
//

#ifndef MRP_MODEL_H
#define MRP_MODEL_H

#include "graph.h"
#include "kruskal.h"
#include "solution.h"

class Model {
    Graph *graph;
    Solution solution;
    MST_Kruskal mst;
    vector<bool> Y;
    vector<double> multipliers;
    vector<vector<int>> A;
    vector<Edge> bestSolution;
    double objectiveValue;
    int originalObjectiveValue, bestIterationDual, bestIterationPrimal, iter = 0;
    double UB, LB, lambda;
    bool feasible;

public:
    Model(Graph *graph);

    double getNorm(vector<double> gradient);

    double getObjValue();

    double getOriginalObjectiveValue();

    bool isFeasible();

    bool solve();

    void getGradient(vector<double> &gradient);

    void updateEdges();

    double lagrangean(int time);

    int initialHeuristic();

    void swapEdgesHeuristic();

    void showSolution(const char *output);

};


#endif //MRP_MODEL_H