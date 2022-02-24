///
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>
using namespace std;
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_des;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


void testcase() {
  int n,m,a,s,c,d; cin>>n>>m>>a>>s>>c>>d;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  edge_desc e;
  for (int i=0; i<m; i++) {
    char w; int x,y,z; cin>>w>>x>>y>>z;
    e = boost::add_edge(x, y, G).first; weights[e] = z;
    if (w=='L') {
      e = boost::add_edge(y, x, G).first; weights[e] = z;
    }
  }
  
  vector<int> agent(a), shelter(s);
  for (int i=0; i<a; i++) cin >> agent[i];
  for (int i=0; i<s; i++) cin >> shelter[i];
  
  vector<vector<vector<int>>> safetime(a, vector<vector<int>>(s, vector<int>(c, INT_MAX)));
  int max_time = 0;
  for (int i=0; i<a; i++) {
    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, agent[i],
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));
    for (int j=0; j<s; j++) {
      if (dist_map[shelter[j]] < INT_MAX) {
        safetime[i][j][0] = dist_map[shelter[j]] + d;
        max_time = max(safetime[i][j][0], max_time);
        if (c == 2) {
          // if x+d >= y --> x+2d >= y+d, max_time = x+2d, x use x+2d edge, y use y+d edge
          // if x+d < y --> x+2d < y+d, max_time = y+2d, x use x+d edge, y use y+2d edge
          // add those four edges, always have a valid matching
          safetime[i][j][1] = dist_map[shelter[j]] + d*2;
          max_time = max(safetime[i][j][1], max_time);
        }
      }
    }
  }
  
  int low=0, high=max_time;
  while (low < high) {
    int mid = (low+high)/2;
    graph matchG(a+s*c);
    for (int i=0; i<a; i++) {
      for (int j=0; j<s; j++) {
        if (safetime[i][j][0] <= mid) {
          //indexing!
          boost::add_edge(i,j+a,matchG);
        }
        if (c == 2) {
          if (safetime[i][j][1] <= mid) {
            //indexing!
            boost::add_edge(i,j+a+s,matchG);
          }
        }
      }
    }
    
    std::vector<vertex_des> mate_map(a+s*c);  // exterior property map
    boost::edmonds_maximum_cardinality_matching(matchG,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, matchG)));
    int matching_size = boost::matching_size(matchG,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, matchG)));
      
    if (matching_size == a) {
      high = mid;
    } else {
      low = mid+1;
    }
  }
  cout << high;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}