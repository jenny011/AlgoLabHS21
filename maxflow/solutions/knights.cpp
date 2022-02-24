///1
#include <iostream>
#include <vector>
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
  int m,n,k,c; cin>>m>>n>>k>>c;
  if (!k) {
    cout << 0;
    return;
  }
  
  graph G(2*n*m);
  edge_adder adder(G);
  const vertex_desc src = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  
  int offset = 2*m;
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      int id = 2*j+i*offset;
      adder.add_edge(id, id+1, c);
      if (i > 0) {
        adder.add_edge(id+1, id-offset, 1);
      } else {
        adder.add_edge(id+1, sink, 1);
      }
      if (i < n-1) {
        adder.add_edge(id+1, id+offset, 1);
      } else {
        adder.add_edge(id+1, sink, 1);
      }
      if (j > 0) {
        adder.add_edge(id+1, id-2, 1);
      }
      if (j < m-1) {
        adder.add_edge(id+1, id+2, 1);
      }
    }
  }
  
  for (int i=0; i<n; i++) {
    int id = i*offset;
    adder.add_edge(id+1, sink, 1);
    adder.add_edge(id+2*m-1, sink, 1);
  }
  
  for (int i=0; i<k; i++) {
    int a,b; cin >> a >> b;
    adder.add_edge(src, a*2 + b*offset, 1);
  }
  
  int flow = push_relabel_max_flow(G, src, sink);
  cout << flow;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}