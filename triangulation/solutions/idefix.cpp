#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <utility>
#include <iomanip>
using namespace std;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef K::Point_2 P;

typedef std::pair<P,Index> IP;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

void testcase() {
  Index n,m; cin >> n >> m;
  K::FT s; cin >> s;
  int k; cin >> k;
  
  vector<IP> trees;
  trees.reserve(n);
  for(Index i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    trees.emplace_back(P(x,y), i);
  }
  
  vector<P> bones;
  bones.reserve(m);
  for(Index i=0; i<m; i++) {
    int x,y; cin >> x >> y;
    bones.emplace_back(P(x,y));
  }
  
  Delaunay t;
  t.insert(trees.begin(), trees.end());
  
  // prepare edges
  EdgeV edges;
  edges.reserve(3*n+m);
  // add tree edges
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  
  // add "bone to nearest tree" edges
  // WHY only consider nearest tree? What if a bone is under many trees?
  // Omit the case where the bone is not even under its nearest tree: the bone can never be visited :(
  // Consider two trees A and B, and a bone is at least under its nearest tree A:
  // 1. two trees are connected: bone, A, B are in the same connected component, 
  //  it doesn't matter whether bone is also under B
  // 2. two trees are not connected: bone, A are in the same connected component,
  //  the disk of A and the disk of B are disconnected, so bone can never be under B
  for (Index i=0; i<m; i++) {
    Vertex_handle j = t.nearest_vertex(bones[i]);
    K::FT elen = 4*CGAL::squared_distance(bones[i], j->point());
    edges.emplace_back(i+n, j->info(), elen);
  }
  
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });

  // DSU for max bones
  boost::disjoint_sets_with_storage<> uf(n+m);
  Index n_components = n+m;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (std::get<2>(*e) > s) break;
    if (c1 != c2) {
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
  
  vector<int> union_count(n+m, 0);
  for (Index i=0; i<m; i++) {
    union_count[uf.find_set(i+n)]++;
  }
  cout << *max_element(union_count.begin(), union_count.end()) << " ";
  
  // min disk radius for at least k bones
  // initialize the count: tree has 0 bone, a bone has 1 bone.
  // so in total m bones to start with
  vector<int> count(n+m);
  for (Index i=0; i<m; i++) {
    count[i+n] = 1;
  }
  
  uf = boost::disjoint_sets_with_storage<>(n+m);
  n_components = n+m;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      uf.link(c1, c2);
      Index c = uf.find_set(std::get<0>(*e));
      // compute the bone count for the connected components
      int count1 = count[c1], count2 = count[c2];
      count[c] += count1+count2;
      count[c1] -= count1;
      count[c2] -= count2;
      if (count[c] >= k) {
        cout << fixed << setprecision(0) << std::get<2>(*e);
        break;
      }
      if (--n_components == 1) break;
    }
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