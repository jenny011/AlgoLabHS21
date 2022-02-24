///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;


void testcase() {
  int m,n; cin>>m>>n;
  vector<P> people, lights;
  vector<int> radius;
  people.reserve(m);
  radius.reserve(m);
  for (int i=0; i<m; i++) {
    int x,y,r; cin>>x>>y>>r;
    people.push_back(P(x,y));
    radius.push_back(r);
  }
  
  K::FT h; cin >> h;
  lights.reserve(n);
  for (int i=0; i<n; i++) {
    int x,y; cin>>x>>y;
    lights.push_back(P(x,y));
  }
  
  vector<bool> final_survive(m, true);
  Triangulation t;
  t.insert(lights.begin(), lights.end());
  
  int out=0;
  for (int i=0; i<m; i++) {
    if (CGAL::squared_distance(people[i], t.nearest_vertex(people[i])->point()) < (h+radius[i])*(h+radius[i])) {
      final_survive[i] = false;
      out++;
    }
  }
  if (out < m) {
    for (int i=0; i<m; i++) {
      if (final_survive[i]) cout << i << " ";
    }
    return;
  }
  
  // logN
  int low=0, high = n;
  while (low < high) {
    int mid = (low+high+1)/2;
    vector<bool> survive(m, true);
    // NlogN
    Triangulation t;
    t.insert(lights.begin(), lights.begin()+mid);
    
    // N
    int out=0;
    for (int i=0; i<m; i++) {
      if (CGAL::squared_distance(people[i], t.nearest_vertex(people[i])->point()) < (h+radius[i])*(h+radius[i])) {
        survive[i] = false;
        out++;
      }
    }
    
    if (out == m) {
      high = mid-1;
    } else {
      final_survive = survive;
      low = mid;
    }
  }
  //in total: Nlog^N
  
  for (int i=0; i<m; i++) {
    if (final_survive[i]) cout << i << " ";
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