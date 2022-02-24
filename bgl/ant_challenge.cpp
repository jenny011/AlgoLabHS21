///1
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <climits>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


void testcase() {
  int n,e,s,a,b; cin >> n >> e >> s >> a >> b;
  
  vector<vector<vector<int>>> local(n, vector<vector<int>>(n, vector<int>(s,-1)));
  for (int i=0; i<e; i++) {
    int u,v; cin >> u >> v;
    for (int i=0; i<s; i++) {
      int w; cin >> w;
      local[min(u,v)][max(u,v)][i] = w;
    }
  }
  vector<int> hives;
  for (int i=0; i<s; i++) {
    int h; cin >> h;
    hives.push_back(h);
  }
  
  vector<vector<int>> global(n, vector<int>(n,INT_MAX));
  for (int i=0; i<s; i++) {
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    edge_desc edge;
    for (int j=0; j<n; j++) {
      for (int k=j; k<n; k++) {
        if (local[j][k][i] != -1) {
          edge = boost::add_edge(j, k, G).first; weights[edge]=local[j][k][i];
        }
      }
    }
    vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(G, back_inserter(mst));
  
    for (vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
      vertex_desc u = min(boost::source(*it, G), boost::target(*it, G));
      vertex_desc v = max(boost::source(*it, G), boost::target(*it, G));
      global[u][v] = min(weights[*it], global[u][v]);
    }
  }
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc edge;
  for (int j=0; j<n; j++) {
    for (int k=j; k<n; k++) {
      if (global[j][k] < INT_MAX) {
        edge = boost::add_edge(j, k, G).first; weights[edge]=global[j][k];
      }
    }
  }
  
  vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, a,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  cout << dist_map[b];
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}