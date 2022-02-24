///
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <utility>
#include <queue>
using namespace std;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Vertex_handle  Vertex_handle;

typedef K::Point_2 P;
typedef std::pair<P,Index> IP;
typedef pair<P,P> PP;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;


// reverse thinking!
// Edges that are not included in Delaunay doesn't need to be checked when coloring
// they are checked here
// A bipartite graph has only one valid partition (coloring)
// Only need to make sure that the coloring is valid, o.w. not bipartite
// Each group of same color vertices should be far enough from each other
bool too_close(vector<P> &points, K::FT r) {
  Delaunay t;
  t.insert(points.begin(), points.end());
  for (auto e=t.finite_edges_begin(); e!=t.finite_edges_end(); e++) {
    if (t.segment(e).squared_length() <= r*r) {
      return true;
    }
  }
  return false;
}

void testcase() {
  Index n, m; K::FT r; cin >> n >> m >> r;
  vector<IP> stations;
  stations.reserve(n);
  for (Index i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    stations.emplace_back(P(x,y), i);
  }
  vector<PP> clues;
  clues.reserve(m);
  for (Index i=0; i<m; i++) {
    int x,y,u,v; cin >> x >> y >> u >> v;
    clues.emplace_back(P(x,y), P(u,v));
  }
  
  if (n == 1) {
    for (Index i=0; i<m; i++) {
      if (CGAL::squared_distance(clues[i].first, clues[i].second) <= r*r) {
        cout << "y";
        continue;
      }
      if (CGAL::squared_distance(clues[i].first, stations[0].first) > r*r || CGAL::squared_distance(clues[i].first, stations[0].first) > r*r) {
        cout << "n";
      } else {
        cout << "y";
      }
    }  
    return;
  }
  
  Delaunay t;
  t.insert(stations.begin(), stations.end());
  
  bool interfere = false;
  vector<int> colors(n, -1);
  vector<P> zero, one;
  queue<pair<int,Vertex_handle>> bfs;
  for (auto u = t.finite_vertices_begin(); u != t.finite_vertices_end(); u++) {
    // Triangulation only includes closest vertex edges
    // so if the bfs is exhausted and we start anew,
    // then the new group is completely out of range of the other groups.
    // all vertices from the new groups are too far from the remaining vertices
    if (colors[u->info()] == -1) {
      bfs.emplace(0,u);
    }
    
    while (!bfs.empty()) {
      int color = bfs.front().first;
      Vertex_handle v = bfs.front().second;
      bfs.pop();
      
      if (colors[v->info()] == -1) {
        colors[v->info()] = color;
        if (color) {
          one.push_back(v->point());
        } else {
          zero.push_back(v->point());
        }
        Delaunay::Vertex_circulator c = t.incident_vertices(v);
        do { 
          // only color the in-range neighbors.
          // out-of-range neighbors might be colored by their close neighbors.
          if (!t.is_infinite(c) && CGAL::squared_distance(c->point(), v->point()) <= r*r) {
            bfs.emplace(1-color, c);
          }
        } while (++c != t.incident_vertices(v));
      } else {
        if (color != colors[v->info()]) {
          interfere = true;
          break;
        }
      }
    }
  }

  if (!interfere) {
    interfere = too_close(zero, r) || too_close(one, r);
  }
  
  if (interfere) {
    for (Index i=0; i<m; i++) cout << "n";
    return;
  }
  
  
  EdgeV edges;
  edges.reserve(3*n);
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });

  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (std::get<2>(*e) > r*r) break;
    if (c1 != c2) {
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
  
  for (Index i=0; i<m; i++) {
    if (CGAL::squared_distance(clues[i].first, clues[i].second) <= r*r) {
      cout << "y";
      continue;
    }
    Vertex_handle s1 = t.nearest_vertex(clues[i].first);
    Vertex_handle s2 = t.nearest_vertex(clues[i].second);
    K::FT maxd = max(CGAL::squared_distance(clues[i].first, s1->point()), CGAL::squared_distance(clues[i].second, s2->point()));
    if (maxd > r*r || uf.find_set(s1->info()) != uf.find_set(s2->info())) {
      cout << "n";
    } else {
      cout << "y";
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