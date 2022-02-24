///2
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <climits>
using namespace std;

#include <boost/pending/disjoint_sets.hpp>

typedef std::tuple<int,int,int> Edge;


void testcase() {
  int n,k; cin >> n >> k;
  vector<Edge> edges;
  for (int i=0; i<n-1; i++) {
    for (int j=i+1; j<n; j++) {
      int c; cin >> c;
      edges.emplace_back(c,i,j);
    }
  }
  sort(edges.begin(), edges.end());
  
  vector<int> mst;
  boost::disjoint_sets_with_storage<> uf(n);
  int n_components = n;
  // ... and process edges in order of increasing length
  for (int i=0; i<(int)edges.size(); i++) {
    // determine components of endpoints
    int c1 = uf.find_set(std::get<1>(edges[i]));
    int c2 = uf.find_set(std::get<2>(edges[i]));
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      mst.push_back(i);
      if (--n_components == 1) break;
    }
  }
  
  int mincost = INT_MAX;
  for (int j=0; j<(int)mst.size(); j++) {
    int cost = 0;
    boost::disjoint_sets_with_storage<> uf(n);
    int n_components = n;
    // ... and process edges in order of increasing length
    for (int i=0; i<(int)edges.size(); i++) {
      if (i == mst[j]) continue;
      // determine components of endpoints
      int c1 = uf.find_set(std::get<1>(edges[i]));
      int c2 = uf.find_set(std::get<2>(edges[i]));
      if (c1 != c2) {
        uf.link(c1, c2);
        cost += std::get<0>(edges[i]);
        if (--n_components == 1) break;
      }
    }
    mincost = min(mincost, cost);
  }
  cout << mincost;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}