#include "lagrange.h"

// heuristic to get an initial primal solution
int LagrangianRelaxation::heuristic(MBVInstance &in, EdgeBoolMap &xValues, NodeBoolMap &yValues){
    EdgeValueMap weights(in.g);
    EdgeBoolMap tmp(in.g);
    int best = 99999;
    int curr;

    for(int i = 0; i < 100; i++){
        // set random weights and run kruskal
        for(EdgeIt e(in.g); e != INVALID; ++e){
            weights[e] = ((double) rand() / (RAND_MAX));
        }
        kruskal(in.g, weights, tmp);

        // count number of branch nodes
        curr = 0;
        for(NodeIt v(in.g); v != INVALID; ++v){
            int count = 0;

            for(IncEdgeIt e(in.g, v); e != INVALID; ++e){
                if(tmp[e]){
                    count++;
                }
            }

            if(count > 2){
                curr++;
            }
        }

        // update if needed
        if(curr < best){
            for(NodeIt v(in.g); v != INVALID; ++v){
                int count = 0;

                for(IncEdgeIt e(in.g, v); e != INVALID; ++e){
                    if(tmp[e]){
                        count++;
                    }

                    xValues[e] = tmp[e];
                }

                if(count > 2){
                    yValues[v] = true;
                }
                else{
                    yValues[v] = false;
                }
            }

            best = curr;
        }
    }

    return best;
}

// the subproblem on the x variables is the MST problem
double LagrangianRelaxation::solveXProblem(MBVInstance &in, EdgeValueMap &weights, EdgeBoolMap &xValues, double &countBranch){
    // run lemon implementation of Kruskal's algorithm
    double totalCost = kruskal(in.g, weights, xValues);

    for(NodeIt v(in.g); v != INVALID; ++v){
        int count = 0;

        for(IncEdgeIt e(in.g, v); e != INVALID; ++e){
            if(xValues[e]){
                count++;
            }
        }

        if(count > 2){
            countBranch++;
        }
    }

    return totalCost;
}

// the subproblem on the y variables can be solved by inspection
double LagrangianRelaxation::solveYProblem(MBVInstance &in, NodeValueMap &weights, NodeBoolMap &yValues){
    double totalCost = 0;

    for(NodeIt v(in.g); v != INVALID; ++v){
        if(weights[v] < -eps){
            yValues[v] = true;
            totalCost += weights[v];
        }
        else{
            yValues[v] = false;
        }
    }

    return totalCost;
}

void LagrangianRelaxation::setWeights(MBVInstance &in, NodeValueMap &lambdas, EdgeValueMap &xWeights, NodeValueMap &yWeights){
    // set edge weights
    for(EdgeIt e(in.g); e != INVALID; ++e){
        xWeights[e] = lambdas[in.g.u(e)] + lambdas[in.g.v(e)];
    }

    // set node weights
    for(NodeIt v(in.g); v != INVALID; ++v){
        yWeights[v] = 1 - in.degrees[v] * lambdas[v];
    }
}


int LagrangianRelaxation::run(MBVInstance &in, Params &params, EdgeBoolMap &solution){
    // initialize lambdas
    NodeValueMap lambdas(in.g);
    for(NodeIt v(in.g); v != INVALID; ++v){
        lambdas[v] = 0.0;
        /*
        if(in.degrees[v] <= 2){
            lambdas[v] = 0.0;
        }
        else{
            lambdas[v] = 1.0 / in.minDegree;
        }*/
    }

    // main loop of the algorithm based on lagrangian relaxation
    NodeValueMap subGradients(in.g);
    EdgeValueMap xWeights(in.g);
    NodeValueMap yWeights(in.g);
    EdgeBoolMap xValues(in.g);
    NodeBoolMap yValues(in.g);
    double xCost, yCost, step, g2, currUB, currLB;
    double bestUB = heuristic(in, solution, yValues);
    double bestLB = 0;

    cout << "heuristic sol cost: " << bestUB << endl;
    cout << "-------------------------------------------------" << endl;

    for(int k = 0; k < params.maxIt && bestUB - bestLB >= 1 + eps; k++){
        // solve subproblems
        currUB = 0;
        setWeights(in, lambdas, xWeights, yWeights);
        xCost = solveXProblem(in, xWeights, xValues, currUB);
        yCost = solveYProblem(in, yWeights, yValues);

        // update primal solution
        if(currUB < bestUB){
            bestUB = currUB;

            for(EdgeIt e(in.g); e != INVALID; ++e){
                solution[e] = xValues[e];
            }
        }

        // compute current LB
        currLB = xCost + yCost;
        for(NodeIt v(in.g); v != INVALID; ++v){
            currLB -= 2 * lambdas[v];
        }
        bestLB = max(bestLB, currLB);

        // compute subgradients
        g2 = 0;
        for(NodeIt v(in.g); v != INVALID; ++v){
            subGradients[v] = -2;

            for(IncEdgeIt e(in.g, v); e != INVALID; ++e){
                subGradients[v] += xValues[e] ? 1 : 0;
            }

            subGradients[v] -= in.degrees[v] * (yValues[v] ? 1 : 0);
            g2 += subGradients[v] * subGradients[v];
        }

        // compute step size
        step = params.precision * ((bestUB - currLB) / g2);

        // update lambdas
        for(NodeIt v(in.g); v != INVALID; ++v){
            if(in.degrees[v] > 2){
                lambdas[v] = max(0.0, lambdas[v] + step * subGradients[v]);
                /*
                if(lambdas[v] + step * subGradients[v] < eps){
                    lambdas[v] = 0;
                }
                else if(lambdas[v] + step * subGradients[v] >= eps && lambdas[v] + step * subGradients[v] <= (1.0 / in.degrees[v])){
                    lambdas[v] = lambdas[v] + step * subGradients[v];
                }
                else{
                    lambdas[v] = (1.0 / in.degrees[v]);
                }*/
            }
        }

        cout << "currLB: " << currLB << endl;
        cout << "currUB: " << currUB << endl;
    }

    cout << "-------------------------------------------------" << endl;
    cout << "LB: " << bestLB << endl;
    cout << "UB: " << bestUB << endl;
    cout << "-------------------------------------------------" << endl;

    return bestUB;
}
