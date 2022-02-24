///5
#include <iostream>
#include <vector>
#include <string>
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
  int h,w; cin >> h >> w;
  graph G(52);
  edge_adder adder(G);
  const vertex_desc src = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  
  vector<int> note_count(26,0);
  string note; cin >> note;
  for (auto c=note.cbegin(); c!=note.cend(); c++) {
    note_count[*c-'A']++;
  }
  
  vector<int> front_count(26, 0);
  vector<string> front(h);
  for (int i=0; i<h; i++) {
    string s; cin >> s;
    front[i] = s;
    for (int j=0; j<w; j++) front_count[s[j]-'A']++;
  }
  
  for (int i=0; i<26; i++) {
    adder.add_edge(src, i, front_count[i]);
    adder.add_edge(i, 26+i, front_count[i]);
    adder.add_edge(26+i, sink, note_count[i]);
  }
  
  vector<vector<int>> count_pair(26, vector<int>(26, 0));
  for (int i=0; i<h; i++) {
    string s; cin >> s;
    for (int j=0; j<w; j++) {
      count_pair[front[i][w-1-j] - 'A'][s[j]-'A']++;
    }
  }
  for (int i=0; i<26; i++) {
    for (int j=0; j<26; j++) {
      if (count_pair[i][j]) {
        adder.add_edge(i, 26+j, count_pair[i][j]);
      }
    }
  }
  
  int flow = push_relabel_max_flow(G, src, sink);
  if (flow == (int)note.size()) {
    cout << "Yes";
  } else {
    cout << "No";
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