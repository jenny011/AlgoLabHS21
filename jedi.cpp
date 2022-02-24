///3
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <climits>
#include <stack>
using namespace std;
#include <boost/pending/disjoint_sets.hpp>

typedef tuple<int,int,int> Edge;

void testcase() {
  int n, T; cin >> n >> T;
  T--;
  vector<Edge> graph;
  int offset = 0;
  for (int i=0; i<n-1; i++) {
    for (int j=i+1; j<n; j++) {
      int v; cin >> v;
      if (i == T) {
        graph.emplace_back(v,-1,j);
      } else if (j == T) {
        graph.emplace_back(v,i,-1);
      } else {
        graph.emplace_back(v,i,j);
      }
      offset++;
    }
  }
  
  sort(graph.begin(), graph.end());

  // save mst edges, and neighbor relationship
  // get initial cost
  vector<bool> mst(offset, false);
  int cost = 0;
  boost::disjoint_sets_with_storage<> uf(n);
  int n_components = n;
  vector<vector<pair<int,int>>> neighbors(n);
  for (int i=0; i<offset; i++) {
    Edge e = graph[i];
    int i1 = std::get<1>(e);
    int i2 = std::get<2>(e);
    if (i1 == -1) i1 = T;
    if (i2 == -1) i2 = T;
    int c1 = uf.find_set(i1);
    int c2 = uf.find_set(i2);
    if (c1 != c2) {
      uf.link(c1, c2);
      cost += std::get<0>(e);
      neighbors[i1].emplace_back(i2,std::get<0>(e));
      neighbors[i2].emplace_back(i1,std::get<0>(e));
      mst[i] = true;
      if (--n_components == 1) break;
    }
  }
  
  // for each vertex, save the most expensive edge along its mst path to every other vertex
  vector<vector<int>> max_edge(n, vector<int>(n, 0));
  for (int i=0; i<n; i++) {
    // for each vertex, do a dfs on the mst to all the other vertices
    vector<bool> visited(n, false);
    stack<tuple<int,int,int>> dfs;
    visited[i] = true;
    for (const auto j : neighbors[i]) {
      dfs.emplace(i, j.first, j.second);
    }
    
    while (!dfs.empty()) {
      int u = get<0>(dfs.top());
      int v = get<1>(dfs.top());
      int l = get<2>(dfs.top());
      dfs.pop();
      visited[v] = true;
      
      // Given an mst tree: i --> u --> v1
      //                      --> v2
      // The maximum edge from i to u is found,
      // the maximum edge from i to v1 is simply max(max_edge[i][u], edge_cost[u][v1])
      // Same for i to v2
      int max_e = max(max_edge[i][u], l);
      // The weight of the max edge from i1 to i2 is unique, 
      // so i2-->i1 overwriting i1-->i2 is okay.
      max_edge[i][v] = max_e;
      max_edge[v][i] = max_e;
      
      // if the vertex is not a leaf
      if (neighbors[v].size() > 1 || (neighbors[v].size() == 1 && neighbors[v][0].first != u)) {
        for (const auto x : neighbors[v]) {
          // stop when all vertices are visited
          if (!visited[x.first]) dfs.emplace(v, x.first, x.second);
        }
      }
    }
  }  
  
  // For each edge (u,v) not in the initial mst,
  // adding it to the graph must produce one and only one cycle: u --mst_path--> v --uv_edge--> u
  // The cheapest way to generate a new mst given that cycle is to 
  // remove the most expensive edge in the mst_path (em) and add the uv_edge
  // So for each edge not in initial mst, we find the minimum diff (cost[em]-cost[uv]) 
  // The cheapest new mst is given by cost + min(minimum diffs)
  int min_diff = INT_MAX;
  for (int i=0; i<offset; i++) {
    if (!mst[i]) {
      Edge e = graph[i];
      int i1 = std::get<1>(e);
      int i2 = std::get<2>(e);
      int l = std::get<0>(e);
      if (i1 == -1) i1 = T;
      if (i2 == -1) i2 = T;
      min_diff = min(min_diff, l-max_edge[i1][i2]);
    }
  }
  cout << cost + min_diff;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}