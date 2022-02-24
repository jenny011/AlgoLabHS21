///
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
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

void add_valid(int i, int j, int n, vector<int> &board, edge_adder &adder, int k) {
  if (i >= 0 && i < n && j >= 0 && j < n && board[i*n+j]) {
    adder.add_edge(k, i*n+j, 1);
  }
}

void testcase() {
  int n; cin >> n;
  graph G(n*n);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const vertex_desc src = boost::add_vertex(G);
  const vertex_desc sink = boost::add_vertex(G);
  vector<int> board(n*n,0);
  
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> board[i*n+j];
    }
  }
  
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      int k = i*n+j;
      if ((i+j)&1 && board[k]) {
        adder.add_edge(k, sink, 1);
      } else if (board[k]) {
        adder.add_edge(src, k, 1);
        add_valid(i-1, j-2, n, board, adder, k);
        add_valid(i-1, j+2, n, board, adder, k);
        add_valid(i+1, j-2, n, board, adder, k);
        add_valid(i+1, j+2, n, board, adder, k);
        add_valid(i-2, j-1, n, board, adder, k);
        add_valid(i-2, j+1, n, board, adder, k);
        add_valid(i+2, j-1, n, board, adder, k);
        add_valid(i+2, j+1, n, board, adder, k);
      }
     
    }
  }
  
  int flow = boost::push_relabel_max_flow(G, src, sink);

  // BFS to find vertex set S
  std::vector<int> vis(n*n, false); // visited flags
  std::queue<int> Q; // BFS queue (from std:: not boost::)
  vis[src] = true; // Mark the source as visited
  Q.push(src);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }
  
  int count=0;
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      if ((!((i+j)&1) && vis[i*n+j] || (i+j)&1 && !vis[i*n+j]) && board[i*n+j]) count++;
    }
  }
  cout << count;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}