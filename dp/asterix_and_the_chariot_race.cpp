#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>
using namespace std;

vector<int> m,p,c;

void dp(vector<int> &costs, unordered_map<int, vector<int>> &stages, int i) {
  if (stages[i].empty()) {
    m[i] = costs[i];
    p[i] = 0;
    return;
  }
  
  for (const auto j : stages[i]) {
    // if (m[i] == INT_MAX) dp(costs, stages, j);
    dp(costs, stages, j);
  }
  
  // vector<int> minmc(stages[i].size());
  m[i] = costs[i];
  p[i] = 0;
  // for (int j=0; j<(int)stages[i].size(); j++) {
  for (const auto j : stages[i]) {
    m[i] += min(p[j], min(m[j], c[j]));
    p[i] += min(m[j], c[j]);
    // minmc[j] = min(m[stages[i][j]], c[stages[i][j]]);
    // m[i] += min(p[stages[i][j]], minmc[j]);
    // p[i] += minmc[j];
  }
  
  int cost = m[stages[i][0]];
  for (int j=1; j<(int)stages[i].size(); j++) {
    // cost += minmc[j];
    cost += min(m[stages[i][j]], c[stages[i][j]]);
  }
  c[i] = cost;
  for (int j=1; j<(int)stages[i].size(); j++) {
    cost -= m[stages[i][j-1]];
    cost += min(m[stages[i][j-1]], c[stages[i][j-1]]);
    cost -= min(m[stages[i][j]], c[stages[i][j]]);
    // cost += minmc[j-1];
    // cost -= minmc[j];
    cost += m[stages[i][j]];
    c[i] = min(c[i], cost);
  }
}

void testcase() {
  int n; cin >> n;
  unordered_map<int, vector<int>> stages;
  vector<int> parent(n, 0);
  for (int i=0; i<n-1; i++) {
    int u,v; cin >> u >> v;
    parent[v] = u;
    stages[u].push_back(v);
  }
  vector<int> costs;
  for (int i=0; i<n; i++) {
    int cost; cin >> cost;
    costs.push_back(cost);
  }
  
  m = vector<int>(n, INT_MAX);
  p = vector<int>(n, INT_MAX);
  c = vector<int>(n, INT_MAX);
  
  dp(costs, stages, 0);
  
  cout << min(m[0], c[0]);
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}