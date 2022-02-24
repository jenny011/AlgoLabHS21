///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <utility>
#include <iomanip>
using namespace std;

typedef vector<long> vl;
typedef unordered_map<int,vector<pair<int,int>>> graph;


void dfs(graph &g, vl &coins, vector<int> &memon, vector<int> &memot, vector<long> &memoc, int i, int l) {
  if (memon[i] != -1) return;
  if (g[i].empty()) {
    memon[i] = 1;
    memot[i] = l*2;
    memoc[i] = coins[i]-l;
    return;
  }
  
  vector<tuple<double,int>> compare; 
  for (auto const &child : g[i]) {
    int id = child.first;
    dfs(g, coins, memon, memot, memoc, id, child.second);
    compare.emplace_back((double)memot[id]/memon[id], id);
  }
  sort(compare.begin(), compare.end());
  int total_t = 0, total_n = 0; long total_c = 0;
  total_t += l;
  total_n += (i && 1);
  total_c += coins[i] - total_t;
  for (auto const &child : compare) {
    int id = get<1>(child);
    total_n += memon[id];
    // variable size!
    total_c += memoc[id] - (double)total_t*memon[id];
    total_t += memot[id];
  }
  total_t += l;
  memon[i] = total_n;
  memot[i] = total_t;
  memoc[i] = total_c;
  return; 
}

void testcase() {
  int n; cin >> n;
  vl coins(n+1);
  coins[0] = 0;
  for (int i=1; i<n+1; i++) {
    cin >> coins[i];
  }
  graph g(n+1);
  for (int i=0; i<n; i++) {
    int u,v,l; cin >> u >> v >> l;
    g[u].emplace_back(v,l);
  }
  
  vector<int> memon(n+1,-1), memot(n+1,-1);
  vector<long> memoc(n+1,-1);
  
  dfs(g, coins, memon, memot, memoc, 0, 0);
  cout << fixed << setprecision(0) << memoc[0];
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}