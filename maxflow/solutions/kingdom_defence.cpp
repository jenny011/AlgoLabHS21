///3
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef  boost::graph_traits<graph>::vertex_descriptor      vertex_desc;
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int l, p; cin >> l >> p;
  graph G(l);
  edge_adder adder(G);
  
  const vertex_desc src = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  
  vector<long> supply(l), demand(l);
  for (int i=0; i<l; i++) {
    cin >> supply[i];
    cin >> demand[i];
  }
  
  for (int i=0; i<p; i++) {
    int u,v,m,c; cin>>u>>v>>m>>c;
    adder.add_edge(u,v,c-m);
    demand[u] += m;
    supply[v] += m;
  }
  
  for (int i=0; i<l; i++) {
    adder.add_edge(src,i,supply[i]);
    adder.add_edge(i,sink,demand[i]);
  }
  
  long flow = push_relabel_max_flow(G, src, sink);
  if (flow == accumulate(demand.begin(), demand.end(), 0)) {
    cout << "yes";
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