#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;


int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n) {
    vector<P> exist;
    exist.reserve(n);
    for (int i=0; i<n; i++) {
      int x,y; cin >> x >> y;
      exist.push_back(P(x,y));
    }
    Triangulation t;
    t.insert(exist.begin(), exist.end());
    
    int m; cin >> m;
    for (int i=0; i<m; i++) {
      int x,y; cin >> x >> y;
      P location(x,y);
      long d = CGAL::squared_distance(location, t.nearest_vertex(location)->point());
      cout << fixed << setprecision(0) << d << endl;
    }
    cin >> n;
  }
  return 0;
}