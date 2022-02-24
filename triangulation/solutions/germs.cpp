#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef K::Point_2 P;

double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(int n) {
  K::FT l,b,r,t; cin>>l>>b>>r>>t;
  if (n == 1) {
    int x,y; cin >> x >> y;
    K::FT min_dead = min(x-l, r-x);
    min_dead = min(min_dead, min(y-b, t-y));
    int dead = ceil_to_double(std::sqrt(min_dead-0.5));
    cout << dead << " ";
    cout << dead << " ";
    cout << dead;
    return;
  }
  
  vector<P> germs;
  germs.reserve(n);
  for (int i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    germs.push_back(P(x,y));
  }
  Triangulation tri;
  tri.insert(germs.begin(), germs.end());
  
  vector<K::FT> dead;
  for (auto v = tri.finite_vertices_begin(); v != tri.finite_vertices_end(); ++v) {
    P p = v->point();
    K::FT min_dead = min((p.x()-l)*(p.x()-l), (r-p.x())*(r-p.x()));
    min_dead = min(min_dead, min((p.y()-b)*(p.y()-b), (t-p.y())*(t-p.y())));
    Triangulation::Edge_circulator c = tri.incident_edges(v);
    do {
      if (!tri.is_infinite(c)) {
        min_dead = min(min_dead, tri.segment(c).squared_length()/4);
      }
    } while (++c != tri.incident_edges(v));
    dead.push_back(min_dead);
  }
  
  sort(dead.begin(), dead.end());
  cout << fixed << setprecision(0) << ceil_to_double(std::sqrt(std::sqrt(dead[0])-0.5)) << " ";
  int mid = (n-1)/2;
  if ((n-1)&1) {
    mid++;
  }
  cout << fixed << setprecision(0) << ceil_to_double(std::sqrt(std::sqrt(dead[mid])-0.5)) << " ";
  cout << fixed << setprecision(0) << ceil_to_double(std::sqrt(std::sqrt(dead[n-1])-0.5));
}

int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n) {
    testcase(n); cout << "\n";
    cin >> n;
  }
  return 0;
}