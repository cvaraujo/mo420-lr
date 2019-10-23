#include "../inc/model.h"

Model::Model(Graph *graph) {
    if (graph != nullptr) {
        this->graph = graph;
        this->mst = MST_Kruskal(*graph);
        this->solution = Solution();
        this->Y = vector<bool>(graph->n);
        this->A = vector<vector<int>>(graph->n, vector<int>());
        this->multipliers = vector<double>(graph->n);
        this->bestSolution = vector<Edge>();
    } else exit(0);
    cout << "Initialized the Model" << endl;
}

int Model::initialHeuristic() {
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
    // cout << "Initial heuristic: " << branches << endl;
    return branches;
}

// TODO: Check this function
void Model::swapEdgesHeuristic(){
    // cout << "Swap edges heuristic" << endl;

    // cout << "Solution edges: " << originalObjectiveValue << endl;

    // for (auto edge : solution.edges)
    //     cout << edge.u << " - " << edge.v << endl;

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

    // cout << "Selected " << u << ", Degree " << minimumBranch << endl; 
    if (u != -1){
        for (int vIndex = 0; vIndex < int(A[u].size()); vIndex++) {
            if (changed) break;
        	v = A[u][vIndex];
    	    adjSize = int(graph->incidenceMatrix[v].size());
            if (adjSize >= 2) {
                // cout << "Adj of u selected (v): " << v << endl;
            	for (int kIndex = 0; kIndex < int(graph->incidenceMatrix[v].size()); kIndex++){
                    k = graph->incidenceMatrix[v][kIndex];
            		if (u != k) {
                        // Checking if edge is already in the solution 
                        for (int e = 0; e < int(solution.edges.size()); e++) {
                            edge = solution.edges[e]; i = edge.u, j = edge.v;
                            if ((i == v && j == k) || (j == v && i == k)) alreadyInSolution = true;
                        }
                        if (!alreadyInSolution){
                			if (int(A[k].size()) >= 3 || int(A[k].size()) == 1) {
                                // cout << "Selected k to change: " << k << endl;
                				// Aresta v - k entra e a aresta u - v sai
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
    // if (changed) {
        // cout << "Heuristic change the solution -> " << originalObjectiveValue << endl;
            // cout << "Solution edges: " << originalObjectiveValue << endl;

        // for (auto edge : solution.edges)
            // cout << edge.u << " - " << edge.v << endl;
        // for (i = 0; i < graph->n; i++) {
        //     cout << i << " = [";
        //     for (auto v : A[i]){
        //         cout << v << ", ";
        //     }
        //     cout << "]" << endl;
        // }
        // getchar();
    // }

}

bool Model::solve() {
    this->Y = vector<bool>(graph->n);
    this->A = vector<vector<int>>(graph->n, vector<int>());
    this->originalObjectiveValue = 0.0;

    // Solve the MST Problem
    this->mst = MST_Kruskal(*graph);
    this->solution = mst.solve();
    
    // for(Edge e: graph->edges) {
    //     cout << e.u << " - " << e.v << " = " << e.weight << endl;
    // }

    int u, v;
    for (auto edge: solution.edges){
        u = edge.u, v = edge.v;
        // cout << u << " -- " << v << ": " << edge.weight << endl;
        A[u].push_back(v), A[v].push_back(u);
        if (int(A[u].size()) == 3) originalObjectiveValue++;
        if(int(A[v].size()) == 3) originalObjectiveValue++;
    }
    objectiveValue = solution.value;
    // cout << "MST: " << solution.value << endl;

    // Inspection problem
    double coeficient = 0;
    for (int i : graph->vertices) {
    	coeficient = 1 - (multipliers[i] * double(graph->incidenceMatrix[i].size()));
        // if (graph->fixed[i]) {
        // 	objectiveValue += coeficient;
        // 	Y[i] = true;
        // } else {
	        if (coeficient <= 0) {
	            Y[i] = true;
	            objectiveValue += coeficient;
	        } else Y[i] = false;   
    	// }
    }

    // cout << "Inspection: " << objectiveValue << endl;
    
    for (int i = 0; i < graph->n; i++){
        objectiveValue -= (2 * multipliers[i]); 
    }

    swapEdgesHeuristic();

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

double Model::lagrangean(int time) {
    auto start = chrono::steady_clock::now();
    auto end = chrono::steady_clock::now();
    double theta, originalObjectiveFunction, objectiveFunctionPPL;
    vector<double> gradient = vector<double>(graph->n);
    double norm;
    UB = initialHeuristic(); // Create a constructive heuristic
    LB = 0;//graph->numFixed;
    
    // bestIterationDual = -1;
    //cout << "(Feasible) Upper Bound = " << UB << ", (Relaxed) Lower Bound = " << LB << endl;

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

    // for (int i = 0; i < graph->n; i++) {
    //     cout << multipliers[i] << ", ";
    // }
    // cout << endl;
    while (chrono::duration_cast<chrono::seconds>(end - start).count() < time) {
        // cout << "Elapsed time: " << chrono::duration_cast<chrono::seconds>(end - start).count() << endl;
        if (solve()){
            // Compute the Upper Bound
            originalObjectiveFunction = getOriginalObjectiveValue();
            // cout << "Original Obj.: " << originalObjectiveFunction << endl;
            if (originalObjectiveFunction < UB) {
                UB = originalObjectiveFunction;
                this->bestSolution = this->solution.edges;
                this->bestIterationPrimal = iter;
                // cout << UB << endl;
                if (UB == 0) return UB;
                if ((UB - LB) / UB <= 0.001) return UB;
            }

            // and Lower Bound
            objectiveFunctionPPL = getObjValue();
            // cout << "PPL: " << objectiveFunctionPPL << endl;
            if (objectiveFunctionPPL > LB) {
                LB = objectiveFunctionPPL;
                bestIterationDual = iter;
            }   
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
                // cout << multipliers[i] << ", ";
            }
            // cout << endl;

            updateEdges();

//            cout << "(Feasible) Upper Bound = " << UB << ", (Relaxed) Lower Bound = " << LB << endl;
            iter++;
            end = chrono::steady_clock::now();
            // cout << iter << endl;
//            getchar();
        }
    }
    // getchar();
    return LB;
}

void Model::showSolution(const char *output) {
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
        fprintf(outputFile, "%d - %lf\n", numberofBranches, UB);   
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
        fprintf(outputFile, "%d %d\n", edge.u+1, edge.v+1);
    
    fclose(outputFile);
}
