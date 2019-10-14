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
    double objectiveValue;
    int originalObjectiveValue;
    double UB, LB, lambda;
    int max_iter;
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

    double lagrangean();

    int heuristic();

};


#endif //MRP_MODEL_H