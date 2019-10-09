#include "../inc/model.h"

Model::Model(Graph *graph) {
    if (graph != nullptr) {
        this->graph = graph;
        this->mst = MST_Kruskal(*graph);
        this->solution = Solution();
        this->Y = vector<bool>(graph->n);
        this->countAdj = vector<int>(graph->n);
        this->A = vector<vector<int>>(graph->n, vector<int>());
        this->multipliers = vector<double>(graph->n);
    } else exit(0);
    cout << "Initialized the Model" << endl;
}

bool Model::solve() {
    this->solution = Solution();
    this->Y = vector<bool>(graph->n);
    this->countAdj = vector<int>(graph->n);
    this->A = vector<vector<int>>(graph->n, vector<int>());
    this->multipliers = vector<double>(graph->n);
    this->originalObjectiveValue = 0;
    this->mst = MST_Kruskal(*graph);
    this->solution = mst.solve();
    
    int u, v;
    for (auto edge: solution.edges){
        u = edge.u, v = edge.v;
        cout << u << " - " << v << " = " << edge.weight << endl;
        countAdj[u]++, countAdj[v]++;
        A[u].push_back(v);//, A[v].push_back(u);
        if (countAdj[u] > 2) originalObjectiveValue++;
        else if(countAdj[v] > 2) originalObjectiveValue++;
    }
    objectiveValue = solution.value;

    for (int i = 0; i < graph->n; i++) {
        objectiveValue += multipliers[i];
        if ((1 - multipliers[i] * countAdj[i]) < 0) {
            this->Y[i] = true;
            objectiveValue += (1 - multipliers[i] * countAdj[i]);
        } else this->Y[i] = false;   
    }

    cout << "Solved" << endl;
    return true;
}

void Model::getGradient(vector<double> &gradient) {
    for (int i = 0; i < graph->n; i++) {
        gradient[i] = int(A[i].size()) - 2;
        if (Y[i]) gradient[i] -= countAdj[i];
        if (gradient[i] < 0) this->feasible = false;
    }
}

double Model::getNorm(vector<double> gradient) {
    double sum = 0;
    for (int k = 0; k < graph->n; k++)
        sum += pow(gradient[k], 2);
    return sqrt(sum);
}

double Model::getObjValue() {
    return this->objectiveValue;
}

double Model::getOriginalObjectiveValue() {
    return originalObjectiveValue;
}

bool Model::isFeasible() {
    if (feasible) return true;
    feasible = true;
    return false;
}
// MST problem
void Model::updateEdges(){
    for (int i = 0; i < graph->n; i++){
        for (int v : A[i]){
            graph->set_edge_value(i, v, multipliers[i]);
        }
    }
}

double Model::lagrangean() {
    int progress = 0, iter = 0;
    double theta, originalObjectiveFunction, objectiveFunctionPPL;
    vector<double> gradient, nextMultipliers = vector<double>(graph->n);
    double norm;
    lambda = 1.5;
    max_iter = 1000;
    B = 3;
    UB = 50; // Create a constructive heuristic
    LB = 0;


    while (iter < max_iter) {
        gradient = vector<double>(graph->n);
        if (solve()){
            getGradient(gradient);

            objectiveFunctionPPL = getObjValue();
            cout << "PPL: " << objectiveFunctionPPL << endl;
            
            norm = getNorm(gradient);

            if (norm == 0) theta = norm;
            else theta = lambda * (UB - objectiveFunctionPPL) / pow(norm, 2);

            nextMultipliers = vector<double>(graph->n);

            for (int i = 0; i < graph->n; i++) {
                nextMultipliers[i] = max(0.0, multipliers[i] + gradient[i] * theta);
                multipliers[i] = nextMultipliers[i];
            }

            if (objectiveFunctionPPL > LB) {
                LB = objectiveFunctionPPL, progress = 0;
            } else progress++;

            if (progress == B) lambda /= 2.0;

            originalObjectiveFunction = getOriginalObjectiveValue();
            cout << "Original Obj: " << originalObjectiveFunction << endl;      
            if (isFeasible() && originalObjectiveFunction < UB) {
                UB = originalObjectiveFunction;
                if ((UB - LB) / LB <= 0.001) return LB;
            }
            updateEdges();
            for(Edge e: graph->edges) {
                cout << e.u << " - " << e.v << " = " << e.weight << endl;
            }
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