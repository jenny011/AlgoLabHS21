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
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Vertex_handle Vertex_handle;
typedef K::Point_2 P;

typedef std::pair<P,Index> IP;
typedef pair<P,P> PP;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

void testcase() {
  Index n,m; K::FT p; cin >> n >> m >> p;
  vector<IP> jammers;
  jammers.reserve(n);
  for (Index i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    jammers.emplace_back(P(x,y), i);
  }
  vector<PP> missions;
  missions.reserve(m);
  for (Index i=0; i<m; i++) {
    int x,y,u,v; cin >> x >> y >> u >> v;
    missions.emplace_back(P(x,y), P(u,v));
  }
  
  Delaunay t;
  t.insert(jammers.begin(), jammers.end());
  
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
    if (std::get<2>(*e) > p) break;
    if (c1 != c2) {
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
  
  K::FT maxa=0, maxb=0;
  boost::disjoint_sets_with_storage<> ufa(n), ufb(n);
  EdgeV::const_iterator ea = edges.begin(), eb = edges.begin();
  Index na = n, nb = n;
  for (Index i=0; i<m; i++) {
    Vertex_handle j1 = t.nearest_vertex(missions[i].first);
    Vertex_handle j2 = t.nearest_vertex(missions[i].second);
    K::FT maxd = 4*max(CGAL::squared_distance(missions[i].first, j1->point()),
                      CGAL::squared_distance(missions[i].second, j2->point()));
                      
    Index i1 = j1->info();
    Index i2 = j2->info();
    if (uf.find_set(i1) == uf.find_set(i2) && maxd <= p) {
      maxb = max(maxb, maxd);
      cout << "y";
      if (ufb.find_set(i1) != ufb.find_set(i2)) {
        for (; eb != edges.end(); ++eb) {
          Index c1 = ufb.find_set(std::get<0>(*eb));
          Index c2 = ufb.find_set(std::get<1>(*eb));
          if (std::get<2>(*eb) > p) break;
          if (c1 != c2) {
            ufb.link(c1, c2);
            if (ufb.find_set(i1) == ufb.find_set(i2)) {
              maxb = max(maxb, std::get<2>(*eb));
              eb++;
              break;
            }
            if (--nb == 1) break;
          }
        }
      }
    } else {
      cout << "n";
    }
    
    maxa = max(maxa, maxd);
    if (ufa.find_set(i1) != ufa.find_set(i2)) {
      for (; ea != edges.end(); ++ea) {
        Index c1 = ufa.find_set(std::get<0>(*ea));
        Index c2 = ufa.find_set(std::get<1>(*ea));
        if (c1 != c2) {
          ufa.link(c1, c2);
          if (ufa.find_set(i1) == ufa.find_set(i2)) {
            maxa = max(maxa, std::get<2>(*ea));
            ea++;
            break;
          }
          if (--na == 1) break;
        }
      }
    }
  }
  cout << "\n";
  cout << fixed << setprecision(0) << maxa << "\n" << maxb;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}
