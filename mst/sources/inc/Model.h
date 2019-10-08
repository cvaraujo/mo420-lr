//
// Created by carlos on 06/03/19.
//

#ifndef MRP_MODEL_H
#define MRP_MODEL_H

#include "Include.h"
#include "Data.h"

class Model {
    Data *data;
    vector<bool> X;
    vector<bool> y;
    vector<double> multipliers;
    double objectiveValue;
    double UB, LB, lambda;
    int max_iter, B;
    bool feasible;

public:
    Model(Data *data);

    double getNorm(vector<double> gradient);

    double getObjValue();

    double originalObjectiveValue();

    bool isFeasible();

    bool solve();

    void getGradient(vector<double> &gradient);

//    void showSolution(const char *input, const char *outputFile, double thetaC, double thetaP, double thetaD,
//    double endTime
//    );

    double lagrangean();

};


#endif //MRP_MODEL_H
