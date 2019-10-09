#include <stdio.h>
#include <string.h>
#include <random>
#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int>ii;

const int MAXV = 1123;

const int MAXVV = 1123456;

int MA[MAXV][MAXV];
double pesos[MAXVV];

int main(void) {
  int n, m, na;
  set<ii> vts;
  memset(MA, 0, sizeof(MA));
  scanf("%d %d", &n, &m);
  printf("%d\n%d\n", n, m);

  //for (int i = 0; i < MAXV; i++) MA[i][i] = 1;

  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> peso(1.0, 200.0);

  for (int i = 0; i < MAXVV; i++) pesos[i] = peso(gen);

  for (int i = 1; i < n; i++) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> vt(0, i - 1);

    int tmp = vt(gen);
    printf("%d %d %.3lf\n", i, tmp, pesos[i - 1]);
    vts.insert(ii(i, tmp));
    vts.insert(ii(tmp, i));
  }

  for (na = n - 1; na < m; ) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> vt(0, n - 1);

    int tmp1 = vt(gen);
    int tmp2 = vt(gen);
    if(vts.find(ii(tmp1, tmp2)) != vts.end()) {
      vts.insert(ii(tmp1, tmp2));
      vts.insert(ii(tmp2, tmp1));
      if (tmp1 != tmp2) printf("%d %d %.3lf\n", tmp1, tmp2, pesos[na + 1]);
      na++;
    }
  }
  return 0;
}
