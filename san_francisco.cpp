///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <queue>
using namespace std;

void testcase() {
  int n,m,k; long x; cin>>n>>m>>x>>k;
  unordered_map<int, vector<pair<int,long>>> graph(n);
  for (int i=0; i<m; i++) {
    int u,v; long p; cin>>u>>v>>p;
    graph[u].emplace_back(v, p);
  }
  
  vector<vector<long>> profit(2, vector<long>(n, 0));
  vector<vector<bool>> at(2, vector<bool>(n, false));
  at[0][0] = true;
  
  for (int i=0; i<k; i++) {
    int curr = i%2;
    int next = 1-curr;
    
    for (int j=0; j<n; j++) {
      if (at[curr][j]) {
        for (const auto &child : graph[j]) {
          at[next][child.first] = true;
          profit[next][child.first] = max(profit[next][child.first], profit[curr][j]+child.second);
          if (profit[next][child.first] >= x) {
            cout << i+1;
            return;
          }
          if (graph[child.first].empty()) {
            at[next][0] = true;
            profit[next][0] = max(profit[next][child.first], profit[next][0]);
          }
        }
      }
    }
    
    profit[curr] = profit[next];
    at[curr] = vector<bool>(n, false);
  }
  
  cout << "Impossible";
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}