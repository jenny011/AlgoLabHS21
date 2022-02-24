#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

vector<vector<int>> students;
int common(int i, int j, int c) {
  vector<int> v_intersection;
  set_intersection(students[i].begin(), students[i].end(),
                    students[j].begin(), students[j].end(),
                    back_inserter(v_intersection));
  return (int)v_intersection.size();
}

void testcase() {
  int n,c,f; cin >> n >> c >> f;
  
  unordered_map<string,int> idmap;
  int id = 0;
  students = vector<vector<int>>(n, vector<int>());
  for (int i=0; i<n; i++) {
    for (int j=0; j<c; j++) {
      string s; cin >> s;
      if (idmap.find(s) == idmap.end()) {
        idmap[s] = id; id++;
      }
      students[i].push_back(idmap[s]);
    }
  }
  for (int i=0; i<n; i++) {
    sort(students[i].begin(), students[i].end());
  }
  
  graph G(n);
  for (int i=0; i<n; i++) {
    for (int j=i+1; j<n; j++) {
      if (common(i,j,c) > f) {
        boost::add_edge(i,j,G);
      }
    }
  }
  
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    
  if (matching_size == n/2) {
    cout << "not optimal";
  } else {
    cout << "optimal";
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
