#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; 
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};


void testcase() {
  int n; cin >> n;
  vector<vector<int>> bonus(n, vector<int>(n));
  int max_bonus = 0;
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> bonus[i][j];
      max_bonus = max(max_bonus, bonus[i][j]);
    }
  }
  
  graph G(n*n*2);
  edge_adder adder(G);
  const vertex_desc src = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  
  // vertex capacity, not edge capacity
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      adder.add_edge(i*n+j, i*n+j+n*n, 1, max_bonus-bonus[i][j]);
      adder.add_edge(i*n+j, i*n+j+n*n, 1, max_bonus);
    }
  }
  
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      if (i+1 < n) {
        adder.add_edge(i*n+j+n*n, (i+1)*n+j, 2, 0);
      }
      if (j+1 < n) {
        adder.add_edge(i*n+j+n*n, i*n+j+1, 2, 0);
      }
    }
  }
  adder.add_edge(n*n-1+n*n, sink, 2, 0);
  adder.add_edge(src, 0, 2, 0);
  
  boost::successive_shortest_path_nonnegative_weights(G, src, sink);
  int cost = boost::find_flow_cost(G);
  cout << (4*n-2)*max_bonus - cost;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}