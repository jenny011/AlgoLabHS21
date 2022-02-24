///1
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
using namespace std;

void testcase() {
  int n,m,k; long x; cin >> n >> m >> x >> k;
  unordered_map<int, vector<pair<int,long>>> graph(n);
  
  for (int i=0; i<m; i++) {
    int u,v; long p; cin >> u >> v >> p;
    graph[u].emplace_back(v,p);
  }
  
  vector<vector<long>> points(2, vector<long>(n, 0));
  vector<vector<bool>> at(2, vector<bool>(n, false));
  at[0][0] = true;
  for (int i=0; i<k; i++) {
    int curr = i%2;
    int next = 1-curr;
    at[next] = vector<bool>(n, false);
    points[next] = vector<long>(n, 0);
    
    for (int u=0; u<n; u++) {
      if (at[curr][u]) {
        if (graph[u].empty()) {
          for (const auto v : graph[0]) {
            at[next][v.first] = true;
            points[next][v.first] = max(points[curr][u] + v.second, points[next][v.first]);
            if (points[next][v.first] >= x) {
              cout << i+1;
              return;
            }
          }
        } else {
          for (const auto v : graph[u]) {
            at[next][v.first] = true;
            points[next][v.first] = max(points[curr][u] + v.second, points[next][v.first]);
            if (points[next][v.first] >= x) {
              cout << i+1;
              return;
            }
          }
        }
      }
    }
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