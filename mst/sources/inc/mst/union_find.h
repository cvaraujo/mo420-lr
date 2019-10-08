#ifndef UNION_FIND_H
#define UNION_FIND_H

#include<vector>

using namespace std;

class UnionFind{
public:
	vector<int> parent;
	vector<int> rank;

	UnionFind(int n);
	void make_set(int x);
	int find_set_default(int i);
	int find_set_path_compression(int i);
	bool same_set_with_find_set_default(int i, int j);
	bool same_set_with_find_path_compression(int i, int j);
	void union_set_with_find_default(int i, int j);
	void union_set_with_path_compression(int i, int j);
	void union_by_rank_with_find_default(int i, int j);
	void union_by_rank_with_path_compression(int i, int j);
};

#endif
