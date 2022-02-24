#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor             vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
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
  int n,m,s; cin>>n>>m>>s;
  graph G(n+m+s);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc sink = boost::add_vertex(G);
  const vertex_desc src = boost::add_vertex(G);
  
  for (int i=0; i<s; i++) {
    int l; cin >> l;
    adder.add_edge(i,sink,l,0);
  }
  for (int i=0; i<m; i++) {
    int si; cin >> si;
    adder.add_edge(i+s,si-1,1,0);
  }
  
  int max_bid=0;
  vector<vector<int>> bids(n, vector<int>(m,0));
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      int b; cin >> b;
      bids[i][j] = b;
      max_bid = max(max_bid, b);
    }
  }
  for (int i=0; i<n; i++) {
    adder.add_edge(src, i+s+m, 1, 0);
    for (int j=0; j<m; j++) {
      adder.add_edge(i+s+m, j+s, 1, max_bid-bids[i][j]);
    }
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, src, sink);
  int cost = boost::find_flow_cost(G);
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  cout << s_flow << " " << - cost + max_bid*s_flow;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}