#include <iostream>
#include <vector>
#include <utility>
using namespace std;
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

namespace boost
{
struct edge_component_t
{
    enum
    {
        num = 555
    };
    typedef edge_property_tag kind;
} edge_component;
}

typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
        boost::property< boost::edge_component_t, std::size_t > >
        graph_t;
typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph_t>::edge_iterator    edge_it;    // to iterate over all edges
typedef boost::graph_traits<graph_t>::edge_descriptor    edge_desc; 

void testcase() {
  int n,m; cin >> n >> m;
  
  graph_t G(n);
  for (int i=0; i<m; i++) {
    int u,v; cin >> u >> v;
    boost::add_edge(u,v,G);
  }
  
  boost::property_map< graph_t, boost::edge_component_t >::type component
        = boost::get(boost::edge_component, G);
  int ncc = boost::biconnected_components(G, component);
  
  vector<vector<edge_desc>> c_edges(ncc, vector<edge_desc>());
  edge_it ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
    c_edges[component[*ei]].push_back(*ei);
  }
  
  vector<pair<int,int>> critical;
  for (int c_i = 0; c_i < ncc; ++c_i){
    // cout << c_edges[c_i].size() << "?\n";
    if ((int)c_edges[c_i].size() == 1) {
      int u = boost::source(c_edges[c_i][0], G);
      int v = boost::target(c_edges[c_i][0], G);
      critical.emplace_back(min(u,v), max(u,v));
    }
  }
  
  cout << critical.size() << "\n";
  if ((int)critical.size() > 0) {
    sort(critical.begin(), critical.end());
    for (const auto &bridge : critical) {
      cout << bridge.first << " " << bridge.second;
      cout << "\n";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase();
  }
  return 0;
}