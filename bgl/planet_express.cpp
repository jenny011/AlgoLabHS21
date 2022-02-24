///2
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <unordered_set>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void testcase() {
  int n,m,k,t; cin >> n >> m >> k>> t;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  vector<int> telenode(t);
  for (int i=0; i<t; i++) {
    cin >> telenode[i];
  }
  
  edge_desc e;
  for (int i=0; i<m; i++) {
    int u,v,c; cin >> u >> v >> c;
    e = boost::add_edge(v,u,G).first; weights[e] = c;
  }
  
  std::vector<int> scc_map(n);  // exterior property map
  // nscc: total number of SCCs
  int nscc = boost::strong_components(G,
    boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

  vector<vector<int>> groups(nscc);
  for (int i=0; i<t; i++) {
    groups[scc_map[telenode[i]]].push_back(telenode[i]);
  }
  
  int dummy_count = 0;
  for (int i=0; i<nscc; i++) {
    int tu = groups[i].size()-1;
    if (tu > 0) {
      vertex_desc dummy = boost::add_vertex(G);
      dummy_count++;
      for (const auto u : groups[i]) {
        e = boost::add_edge(dummy,u,G).first; weights[e] = tu;
        e = boost::add_edge(u,dummy,G).first; weights[e] = 0;
      }
    }
  }
  
  std::vector<int> dist_map(n+dummy_count);
  boost::dijkstra_shortest_paths(G, n-1,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int min_time = INT_MAX;
  for (int i=0; i<k; i++) {
    min_time = min(min_time, dist_map[i]);
  }

  if (min_time <= 1e6) {
    cout << min_time;
  } else {
    cout << "no";
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}