#include "../inc/subgradient.h"

Subgradient::Subgradient(Graph *graph) {
    if (graph != nullptr) {
        this->graph = graph;
        this->mst = MST_Kruskal(*graph);
        this->solution = Solution();
        this->Y = vector<bool>(graph->n);
        this->A = vector<vector<int>>(graph->n, vector<int>());
        this->multipliers = vector<double>(graph->n);
        this->bestSolution = vector<Edge>();
    } else exit(0);
}

int Subgradient::initialHeuristic() {
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

    int u, v;
    for (auto edge : solution.edges) {
        u = edge.u, v = edge.v;
        countIncidence[u]++, countIncidence[v]++;
        if (countIncidence[u] == 3) branches++;
        if (countIncidence[v] == 3) branches++;
    }
    this->bestSolution = solution.edges;
    this->bestIterationPrimal = -1;
    return branches;
}

void Subgradient::swapEdgesHeuristic(){
    int u = -1, v, k, adjSize, i, j;
    bool changed = false, alreadyInSolution = false;
    Edge edge;
    for (auto v : graph->vertices) {
        adjSize = int(A[v].size());
        if (adjSize == 3) {
            u = v;
            break;
        }   
    }

    if (u != -1){
        for (int vIndex = 0; vIndex < int(A[u].size()); vIndex++) {
            if (changed) break;
            v = A[u][vIndex];
            adjSize = int(graph->incidenceMatrix[v].size());
            if (adjSize >= 2) {
                for (int kIndex = 0; kIndex < int(graph->incidenceMatrix[v].size()); kIndex++){
                    k = graph->incidenceMatrix[v][kIndex];
                    if (u != k) {
                        for (int e = 0; e < int(solution.edges.size()); e++) {
                            edge = solution.edges[e]; i = edge.u, j = edge.v;
                            if ((i == v && j == k) || (j == v && i == k)) alreadyInSolution = true;
                        }
                        if (!alreadyInSolution){
                            if (int(A[k].size()) >= 3 || int(A[k].size()) == 1) {
                                // Replace the vertex u, in adjacence list of v, by k
                                for (int uIndex = 0; uIndex < int(A[v].size()); uIndex++)
                                    if (A[v][uIndex] == u) A[v][uIndex] = k;

                                // Remove the vertex v in adjacence list of u
                                A[u].erase(A[u].begin() + vIndex);

                                // Insert in adjacence list of k, the vertex v
                                A[k].push_back(v);

                                // Change the edge on the solution
                                Edge edge;
                                for (int e = 0; e < int(solution.edges.size()); e++) {
                                    edge = solution.edges[e];
                                    i = edge.u, j = edge.v;
                                    if ((i == u && j == v) || (j == u && i == v)) {
                                        solution.edges[e].u = v, solution.edges[e].v = k;
                                        break;
                                    }
                                }
                                changed = true;

                                originalObjectiveValue--;
                                break;
                            }
                        }
                    }
                    if (changed) break;
                }
            }
        }
    }

}

bool Subgradient::solve() {
    this->Y = vector<bool>(graph->n);
    this->A = vector<vector<int>>(graph->n, vector<int>());
    this->originalObjectiveValue = 0.0;

    // Solve the MST Problem
    this->mst = MST_Kruskal(*graph);
    this->solution = mst.solve();
    
    int u, v;
    for (auto edge: solution.edges){
        u = edge.u, v = edge.v;
        A[u].push_back(v), A[v].push_back(u);
        if (int(A[u].size()) == 3) originalObjectiveValue++;
        if(int(A[v].size()) == 3) originalObjectiveValue++;
    }
    objectiveValue = solution.value;

    // Inspection problem
    double coeficient = 0;
    for (int i : graph->vertices) {
        coeficient = 1 - (multipliers[i] * double(graph->incidenceMatrix[i].size()));
         if (graph->fixed[i]) {
            objectiveValue += coeficient;
            Y[i] = true;
         } else {
            if (coeficient <= 0) {
                Y[i] = true;
                objectiveValue += coeficient;
            } else Y[i] = false;   
         }
    }
    
    for (int i = 0; i < graph->n; i++){
        objectiveValue -= (2 * multipliers[i]); 
    }

    swapEdgesHeuristic();

    return true;
}

void Subgradient::getGradient(vector<double> &gradient) {
    for (int i : graph->vertices) {
        gradient[i] = int(A[i].size()) - 2 - (int(graph->incidenceMatrix[i].size()) * Y[i]);
    }
}

double Subgradient::getNorm(vector<double> gradient) {
    double sum = 0;
    for (int i : graph->vertices){
        sum += pow(gradient[i], 2);
    }
    return sqrt(sum);
}

double Subgradient::getObjValue() {
    return this->objectiveValue;
}

double Subgradient::getOriginalObjectiveValue() {
    return originalObjectiveValue;
}

// MST problem
void Subgradient::updateEdges(){
    for (int i = 0; i < graph->m; i++){
        Edge e = graph->edges[i];
        graph->edges[i].weight = multipliers[e.u] + multipliers[e.v];
    }
}

double Subgradient::lagrangean(int time) {
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    double theta, originalObjectiveFunction, objectiveFunctionPPL;
    vector<double> gradient = vector<double>(graph->n);
    double norm;
    UB = initialHeuristic();
    LB = graph->numFixed;
    lambda = 1.5;
    int maxWithoutImprovement = 500;
    
    bestIterationDual = -1;
    
    double min_neigh = graph->n;
    for (int i : graph->vertices) {
        if (int(graph->incidenceMatrix[i].size()) < min_neigh) {
            min_neigh = double(graph->incidenceMatrix[i].size());
        }
    }

    for (int i : graph->vertices) {
        multipliers[i] = 1.0 / min_neigh;
    }

    for (int i = 0; i < graph->m; i++){
        Edge e = graph->edges[i];
        graph->edges[i].weight = multipliers[e.u] + multipliers[e.v];
    }

    while (chrono::duration_cast<chrono::seconds>(end - start).count() < time) {

        if (solve()){
            // Compute the Upper Bound
            originalObjectiveFunction = getOriginalObjectiveValue();
            
            if (originalObjectiveFunction < UB) {
                UB = originalObjectiveFunction;
                this->bestSolution = this->solution.edges;
                this->bestIterationPrimal = iter;
                if (UB == 0) return UB;
                if ((UB - LB) / UB <= 0.001) return UB;
            }

            // and Lower Bound
            objectiveFunctionPPL = getObjValue();

            if (objectiveFunctionPPL > LB) {
                LB = objectiveFunctionPPL;
                bestIterationDual = iter;
                progress = 0;
            } else progress++;
    
            if (progress == maxWithoutImprovement) {
                lambda /= 2.0;
                progress = 0;
            }
            // Get the subgradient
            getGradient(gradient);
            norm = getNorm(gradient);
            
            if (norm == 0) theta = norm;
            else theta = lambda * ((UB - objectiveFunctionPPL) / pow(norm, 2));
            
            for (int i : graph->vertices){
                multipliers[i] = max(0.0, multipliers[i] + gradient[i] * theta);
                if (multipliers[i] > 1/double(graph->incidenceMatrix[i].size()))
                    multipliers[i] = 1/double(graph->incidenceMatrix[i].size());
            }
            updateEdges();
            iter++;
            end = chrono::steady_clock::now();
        }
    }
    fclose(outputFile);
    return LB;
}

void Subgradient::showSolution(const char *output) {
    vector<int> branches = vector<int>(graph->n);
    int numberofBranches = 0;
    for (auto edge : bestSolution) {
        branches[edge.u]++, branches[edge.v]++;
        if (branches[edge.u] == 3) numberofBranches++;
        if (branches[edge.v] == 3) numberofBranches++;    
    }

    if (numberofBranches != UB) {
        FILE *outputFile;
        outputFile = fopen(output, "w");
        fprintf(outputFile, "Something is wrong!\n");
    }

    FILE *outputFile;
    outputFile = fopen(output, "w");
    // Best Dual solution
    fprintf(outputFile, "%.6lf\n", this->LB);
    // Iteration of best dual
    fprintf(outputFile, "%d\n", this->bestIterationDual);
    // Best primal solution
    fprintf(outputFile, "%.0lf\n", this->UB);
    // Iteration of best Primal
    fprintf(outputFile, "%d\n", this->bestIterationPrimal);
    // Number of iterations
    fprintf(outputFile, "%d\n", this->iter);
    // Edges of the best solution
    for (auto edge : bestSolution)
        fprintf(outputFile, "%d %d\n", edge.u, edge.v);
    
    fclose(outputFile);
}