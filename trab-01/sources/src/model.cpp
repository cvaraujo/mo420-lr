#include "../inc/model.h"

Model::Model(Graph *graph) {
    if (graph != nullptr) {
        this->graph = graph;
        this->mst = MST_Kruskal(*graph);
        this->solution = Solution();
        this->Y = vector<bool>(graph->n);
        this->A = vector<vector<int>>(graph->n, vector<int>());
        this->multipliers = vector<double>(graph->n);
    } else exit(0);
    cout << "Initialized the Model" << endl;
}
// TODO: Cutt the edges in half
int Model::heuristic() {
    vector<Edge> edges = vector<Edge>();
    vector<int> countIncidence = vector<int>(graph->n);
    Edge aux;
    int excessEdges, branches = 0;
    for (int u = 0; u < graph->n; u++){
        excessEdges = int(graph->incidenceMatrix[u].size())-2;
        if (excessEdges <= 0) {
            for (int v : graph->incidenceMatrix[u]) {
                if (u < v){
                    aux.u = u, aux.v = v, aux.weight = 0;
                    edges.push_back(aux);
                }
            }
        } else {
            for (int v : graph->incidenceMatrix[u]) {
                if (u < v){
                    if (excessEdges > 0) {
                        aux.u = u, aux.v = v, aux.weight = 1;
                        excessEdges--;
                        edges.push_back(aux);
                    } else {
                        aux.u = u, aux.v = v, aux.weight = 0;
                        edges.push_back(aux);
                    }
                } else excessEdges--;
            }
        }
    }

    this->graph->edges = edges;
    this->mst = MST_Kruskal(*graph);
    this->solution = mst.solve();

    // for (auto edge : graph->edges) {
    //     cout << edge.u << " - " << edge.v << " = " << edge.weight << endl;
    // }

    int u, v;
    for (auto edge : solution.edges) {
        u = edge.u, v = edge.v;
        // cout << u << " -- " << v << endl;
        countIncidence[u]++, countIncidence[v]++;
        if (countIncidence[u] == 3) branches++;
        if (countIncidence[v] == 3) branches++;
    }
    // cout << "heuristic: " << branches << endl;
    // getchar();
    return branches;
}

bool Model::solve() {
    cout << "Solve" << endl;
    this->Y = vector<bool>(graph->n);
    this->A = vector<vector<int>>(graph->n, vector<int>());
    this->originalObjectiveValue = 0.0;

    // Solve the MST Problem
    this->mst = MST_Kruskal(*graph);
    this->solution = mst.solve();
    
    for(Edge e: graph->edges) {
        cout << e.u << " - " << e.v << " = " << e.weight << endl;
    }

    int u, v;
    for (auto edge: solution.edges){
        u = edge.u, v = edge.v;
        // cout << u << " -- " << v << ": " << edge.weight << endl;
        A[u].push_back(v), A[v].push_back(u);
        if (int(A[u].size()) == 3) originalObjectiveValue++;
        if(int(A[v].size()) == 3) originalObjectiveValue++;
    }
    objectiveValue = solution.value;
    cout << "MST: " << solution.value << endl;

    // Inspection problem
    double coeficient = 0;
    for (int i : graph->vertices) {
        coeficient = 1 - (multipliers[i] * double(graph->incidenceMatrix[i].size()));
        if (coeficient <= 0) {
            this->Y[i] = true;
            objectiveValue += coeficient;
        } else this->Y[i] = false;   
    }

    cout << "Inspection: " << objectiveValue << endl;
    
    for (int i = 0; i < graph->n; i++){
        cout << multipliers[i] << ", ";
        objectiveValue -= (2 * multipliers[i]); 
    }
    cout << endl;
    cout << "Final: " << objectiveValue << endl;
    // getchar();
    return true;
}

void Model::getGradient(vector<double> &gradient) {
    for (int i : graph->vertices) {
        gradient[i] = int(A[i].size()) - 2 - (int(graph->incidenceMatrix[i].size()) * Y[i]);
    }
}

double Model::getNorm(vector<double> gradient) {
    double sum = 0;
    for (int i : graph->vertices){
        sum += pow(gradient[i], 2);
    }
    return sqrt(sum);
}

double Model::getObjValue() {
    return this->objectiveValue;
}

double Model::getOriginalObjectiveValue() {
    return originalObjectiveValue;
}

bool Model::isFeasible() {
    for (int i : graph->vertices)
        if (int(A[i].size())-2 > int(graph->incidenceMatrix[i].size()) * Y[i])
            return false;
    return true;
}
// MST problem
void Model::updateEdges(){
    for (int i = 0; i < graph->m; i++){
        Edge e = graph->edges[i];
        graph->edges[i].weight = multipliers[e.u] + multipliers[e.v];
    }
    // for (int u : graph->vertices) {
    //     for (int v : A[u]){
    //         graph->set_edge_value(u, v, multipliers[u] + multipliers[v]);
    //     }
    // }
}

double Model::lagrangean() {
    int iter = 0;
    double theta, originalObjectiveFunction, objectiveFunctionPPL;
    vector<double> gradient = vector<double>(graph->n);
    double norm;
    max_iter = 1000;
    UB = heuristic(); // Create a constructive heuristic
    LB = 0;

    double min_neigh = graph->n;
    for (int i : graph->vertices) {
        if (int(graph->incidenceMatrix[i].size()) < min_neigh) {
            min_neigh = double(graph->incidenceMatrix[i].size());
        }
    }

    for (int i = 0; i < graph->m; i++){
        Edge e = graph->edges[i];
        if (double(graph->incidenceMatrix[e.u].size()) > 2 && double(graph->incidenceMatrix[e.v].size()) > 2) {
            multipliers[e.u] = multipliers[e.v] = 1/min_neigh;
        } else if (double(graph->incidenceMatrix[e.u].size()) > 2) {
            multipliers[e.u] = 1/min_neigh, multipliers[e.v] = 0;
        } else {
            multipliers[e.v] = 1/min_neigh, multipliers[e.u] = 0;
        }
        graph->edges[i].weight = multipliers[e.u] + multipliers[e.v];
    }

    // for (auto edge : graph->edges) {
    //     cout << edge.u << " -- " << edge.v << " = " << edge.weight << endl;
    // }
    // getchar();
    while (iter < max_iter) {
        if (solve()){
            // Compute the Upper Bound
            originalObjectiveFunction = getOriginalObjectiveValue();
            cout << "Original Obj.: " << originalObjectiveFunction << endl;
            if (UB == 0) return UB;
            if (isFeasible() && originalObjectiveFunction < UB) {
                UB = originalObjectiveFunction;
                cout << "(Feasible) Upper Bound = " << UB << ", (Relaxed) Lower Bound = " << LB << endl;
                if (UB == 0) return UB;
                if ((UB - LB) / UB <= 0.001) return UB;
            }

            // and Lower Bound
            objectiveFunctionPPL = getObjValue();
            cout << "PPL: " << objectiveFunctionPPL << endl;
            if (objectiveFunctionPPL > LB) LB = objectiveFunctionPPL;

            // Get the subgradient
            getGradient(gradient);
            norm = getNorm(gradient);
            
            lambda = 0.1;

            if (norm == 0) theta = norm;
            else theta = lambda * ((UB - objectiveFunctionPPL) / pow(norm, 2));
            
            for (int i : graph->vertices){
                multipliers[i] = max(0.0, multipliers[i] + gradient[i] * theta);
                if (multipliers[i] > 1/double(graph->incidenceMatrix[i].size()))
                    multipliers[i] = 1/double(graph->incidenceMatrix[i].size());
            }

            updateEdges();

            cout << "(Feasible) Upper Bound = " << UB << ", (Relaxed) Lower Bound = " << LB << endl;
            iter++;
            getchar();
        }
    }
    return LB;
}

//
//void Model::showSolution(const char *input, const char *outputFile, double thetaC, double thetaP, double thetaD,
//                         double endTime) {
//    try {
//
//        double fo = 0;
//        int i, j;
//        for (auto *arc : data->nonDirectedArcs) {
//            i = arc->getO(), j = arc->getD();
//            if (cplex.getValue(this->x[i][j]) >= 0.5) {
//                //printf("[%d, %d], ", i, j);
//                fo += (10e5 * thetaC) + (arc->getDelay() * (10e5 * thetaP)) -
//                      arc->getEstimateLinkDuration() * thetaD;
//            }
//        }
//
//        FILE *output;
//        output = fopen(outputFile, "a");
//        fprintf(output, "%s: ", input);
//
//        fprintf(output,
//                " ---- CPLEX_FO = %g ---- FO = %lf ---- Time = %g ---- GAP = %g ---- LB = %g\n",
//                UB, fo, endTime, (UB - LB) / UB, LB);
//        fclose(output);
//    } catch (IloException &exception) {
//        FILE *output;
//        output = fopen(outputFile, "a");
//        fprintf(output, "%s: ", input);
//
//        fprintf(output,
//                " ---- FO = %lf ---- GAP = %g ---- LB = %g\n",
//                UB, (UB - LB) / UB, LB);
//        fclose(output);
//        cout << "Write: " << exception.getMessage() << endl;
//    }
//}
