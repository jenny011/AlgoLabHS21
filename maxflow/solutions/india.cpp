#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
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
  int c,g; long b; int k,a; cin>>c>>g>>b>>k>>a;
  vector<pair<int,int>> guides(g);
  vector<int> capacity(g);
  vector<int> price(g);
  int total_capacity = 0;
  for (int i=0; i<g; i++) {
    int x,y; cin>>x>>y;
    guides[i] = pair<int,int>(x,y);
    cin >> price[i];
    cin >> capacity[i];
    total_capacity += capacity[i];
  }
  
  graph nG(c);
  edge_adder nadder(nG);
  auto c_map = boost::get(boost::edge_capacity, nG);
  auto rc_map = boost::get(boost::edge_residual_capacity, nG);
  const vertex_desc nsrc = boost::add_vertex(nG);
  const vertex_desc nsink = boost::add_vertex(nG);
  
  nadder.add_edge(nsrc, k, total_capacity, 0);
  nadder.add_edge(a, nsink, total_capacity, 0);
  for (int i=0; i<g; i++) {
    nadder.add_edge(guides[i].first, guides[i].second, capacity[i], price[i]);
  }
  boost::successive_shortest_path_nonnegative_weights(nG, nsrc, nsink);
  long cost = boost::find_flow_cost(nG);
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(nsrc,nG), nG); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  
  if (cost <= b) {
    cout << s_flow;
    return;
  } else {
    total_capacity = s_flow;
  }
  
  int low=0, high=total_capacity;
  long flow = 0;
  while (low < high) {
    int mid = (low+high+1)/2;
    graph G(c);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc sink = boost::add_vertex(G);
    
    //vertex_desc src = boost::add_vertex(G);
    // adder.add_edge(src,k,middle,0);
    // boost::successive_shortest_path_nonnegative_weights(G, src, a);
    adder.add_edge(src, k, mid, 0);
    adder.add_edge(a, sink, mid, 0);
    for (int i=0; i<g; i++) {
      adder.add_edge(guides[i].first, guides[i].second, capacity[i], price[i]);
    }
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    long cost = boost::find_flow_cost(G);
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    
    if (cost <= b) {
      flow = s_flow;
      low = mid;
    } else {
      high = mid-1;
    }
  }
  
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