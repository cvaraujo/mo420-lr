#include "../inc/union_find.h"

UnionFind::UnionFind(int n) {
    parent.resize(n);
    rank.resize(n);
}

void UnionFind::make_set(int x) {
    parent[x] = x;
    rank[x] = 0;
}

int UnionFind::find_set_default(int i) {
    return (parent[i] == i) ? i : find_set_default(parent[i]);
}

int UnionFind::find_set_path_compression(int i) {
    return (parent[i] == i) ? i : (parent[i] = find_set_path_compression(parent[i]));
}

bool UnionFind::same_set_with_find_set_default(int i, int j) {
    return find_set_default(i) == find_set_default(j);
}

bool UnionFind::same_set_with_find_path_compression(int i, int j) {
    return find_set_path_compression(i) == find_set_path_compression(j);
}

void UnionFind::union_set_with_find_default(int i, int j) {
    int x, y;
    x = find_set_default(i);
    y = find_set_default(j);
    if (x != y) {
       parent[x] = y;
   }
}

void UnionFind::union_set_with_path_compression(int i, int j) {
    int x, y;
    x = find_set_path_compression(i);
    y = find_set_path_compression(j);
    if (x != y) {
       parent[x] = y;
   }
}

void UnionFind::union_by_rank_with_find_default(int i, int j) {
    int x, y;
    x = find_set_default(i);
    y = find_set_default(j);
    if( x != y) {
        if (rank[x] > rank[y]) {
            parent[y] = x;
        } else {
            parent[x] = y;
            if (rank[x] == rank[y]) rank[y]++;
        }
    }
}

void UnionFind::union_by_rank_with_path_compression(int i, int j) {
    int x, y;
    x = find_set_path_compression(i);
    y = find_set_path_compression(j);
    if( x != y) {
        if (rank[x] > rank[y]) {
            parent[y] = x;
        } else {
            parent[x] = y;
            if (rank[x] == rank[y]) rank[y]++;
        }
    }
}