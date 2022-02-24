///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
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
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef K::Point_2 P;
typedef std::pair<P,Index> IPoint;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;


int valid_family_count(vector<int> &ncount, int k) {
  int c = ncount[k-1];
  int n1 = ncount[0];
  int n2 = ncount[1];
  int n3 = ncount[2];
  if (k == 2) {
    c += n1/2;
  } else if (k == 3) {
    c += min(n1,n2) + (n1-min(n1,n2))/3 + (n2-min(n1,n2))/2;
  } else if (k == 4) {
    // don't reuse a variable when it's updated
    int min13 = min(n1,n3);
    c += min13 + n2/2;
    n3 -= min13;
    n1 -= min13;
    if (n3) {
      c += n2&1;
      c += (n3 - (n2&1))/2;
    } else if (n1 >= 2) {
      c += n2&1;
      c += (n1 - 2*(n2&1))/4;
    }
  }
  return c;
}

void testcase() {
  int n,k,f0; K::FT s0; cin>>n>>k>>f0>>s0;
  vector<IPoint> tents;
  tents.reserve(n);
  for (int i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    tents.emplace_back(P(x,y), i);
  }
  Triangulation t;
  t.insert(tents.begin(), tents.end());
  
  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });

  // f0 --> max s
  vector<int> ncount(4,0);
  ncount[0] = n;
  vector<int> count(n, 1);
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      uf.link(c1, c2);
      Index c = uf.find_set(std::get<0>(*e));
      int count1 = count[c1], count2 = count[c2];
      count[c] += count1+count2;
      count[c1] -= count1;
      count[c2] -= count2;
      ncount[min(k, count1)-1]--;
      ncount[min(k, count2)-1]--;
      ncount[min(k, count[c])-1]++;
      if (valid_family_count(ncount, k) < f0) {
        cout << fixed << setprecision(0) << std::get<2>(*e) << " ";
        break;
      }
      if (--n_components == 1) break;
    }
  }
  
  
  //s0 -> f
  ncount = vector<int>(4,0);
  ncount[0] = n;
  count = vector<int>(n, 1);
  uf = boost::disjoint_sets_with_storage<>(n);
  n_components = n;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (std::get<2>(*e) >= s0) break;
    if (c1 != c2) {
      uf.link(c1, c2);
      Index c = uf.find_set(std::get<0>(*e));
      int count1 = count[c1], count2 = count[c2];
      count[c] += count1+count2;
      count[c1] -= count1;
      count[c2] -= count2;
      ncount[min(k, count1)-1]--;
      ncount[min(k, count2)-1]--;
      ncount[min(k, count[c])-1]++;
      if (--n_components == 1) break;
    }
  }
  
  cout << valid_family_count(ncount, k);
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}