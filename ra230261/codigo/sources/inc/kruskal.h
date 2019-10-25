#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "solution.h"
#include "union_find.h"
#include <algorithm>

using namespace std;

class MST_Kruskal{
public:

	Graph graph;

	MST_Kruskal() {}
	MST_Kruskal(Graph graph);

	Solution solve();

	Solution solve(vector<Edge> edges, UnionFind *set);

};

#endif

