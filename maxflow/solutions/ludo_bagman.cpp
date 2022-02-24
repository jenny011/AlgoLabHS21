#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
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
  int e,w,m,d; cin>>e>>w>>m>>d;
  int p,l; cin>>p>>l;
  if (p - e*l < 0 || p - w*l < 0) {
    // read all inputs before return
    for (int i=0; i<m+d; i++) {
      int u,v,r; cin >> u >> v >> r;
    }
    cout << "No schedule!";
    return;
  }
  
  graph G((e+w)*2);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
  const vertex_desc real_src = boost::add_vertex(G);
  const vertex_desc src = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  const vertex_desc extrae = boost::add_vertex(G);
  const vertex_desc extraw = boost::add_vertex(G);
  
  int offset = e+w;
  for (int i=0; i<m; i++) {
    int u,v,r; cin >> u >> v >> r;
    adder.add_edge(u,v+e,1,r);
  }
  for (int i=0; i<d; i++) {
    int u,v,r; cin >> u >> v >> r;
    adder.add_edge(u+offset,v+e+offset,1,r);
  }
  
  for (int i=0; i<e; i++) {
    adder.add_edge(src,i,l,0);
    adder.add_edge(extrae,i,p-e*l,0);
    adder.add_edge(extrae,i+offset,p-e*l,0);
  }
  for (int i=0; i<w; i++) {
    adder.add_edge(i+e,sink,l,0);
    adder.add_edge(i+e,extraw,p-w*l,0);
    adder.add_edge(i+e+offset,extraw,p-w*l,0);
  }
  adder.add_edge(real_src, src, p, 0);
  adder.add_edge(src,extrae,p-e*l,0);
  adder.add_edge(extraw,sink,p-w*l,0);
  
  boost::successive_shortest_path_nonnegative_weights(G, real_src, sink);
  int s_flow = 0;
  out_edge_it et, eend;
  for(boost::tie(et, eend) = boost::out_edges(boost::vertex(real_src,G), G); et != eend; ++et) {
      s_flow += c_map[*et] - rc_map[*et];     
  }
  if (s_flow < p) {
    cout << "No schedule!";
  } else {
    int cost = boost::find_flow_cost(G);
    cout << cost;
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