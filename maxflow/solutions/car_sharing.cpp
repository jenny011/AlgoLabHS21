///5
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
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

typedef std::tuple<int,int,int,int,int> Req;

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
  int n,s; std::cin >> n >> s;
  std::vector<int> supply(s);
  for (int i=0; i<s; i++) {
    std::cin >> supply[i];
  }
  const int total_supply = accumulate(supply.begin(), supply.end(), 0);
  
  std::vector<Req> reqs;
  reqs.reserve(n);
  std::vector<std::map<int,int>> index(s);
  for (int i=0; i<n; i++) {
    int s,t,d,a,p; std::cin>>s>>t>>d>>a>>p;
    s--; t--;
    reqs.emplace_back(s,t,d,a,p);
    index[s].emplace(d,0);
    index[t].emplace(a,0);
  }
  
  int count = 0;
  for (int i=0; i<s; i++) {
    for (auto &id : index[i]) {
      id.second = count++;
    }
  }
  
  graph G(count);
  edge_adder adder(G);
  const vertex_desc src = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  
  for (const auto &req : reqs) {
    int d = std::get<2>(req);
    int a = std::get<3>(req);
    adder.add_edge(index[std::get<0>(req)][d], index[std::get<1>(req)][a], 1, 100*(a-d)-std::get<4>(req));
  }
  
  for (int i=0; i<s; i++) {
    if (index[i].empty()) {
      adder.add_edge(src, sink, supply[i], 100*1e5);
    } else {
      auto prev_id = index[i].cbegin();
      adder.add_edge(src, prev_id->second, supply[i], 100*(prev_id->first));
      for (auto id = ++index[i].cbegin(); id!=index[i].cend(); ++id) {
        adder.add_edge(prev_id->second, id->second, total_supply, 100*(id->first - prev_id->first));
        prev_id = id;
      }
      adder.add_edge(prev_id->second, sink, total_supply, 100*(100000 - prev_id->first));
    }
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, src, sink);
  long cost = 100l*100000*total_supply - boost::find_flow_cost(G);
  std::cout << std::fixed << std::setprecision(0) << cost;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int tc=0; tc<t; tc++) {
    testcase(); std::cout << "\n";
  }
  return 0;
}
