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
  
  // not necessary
  // Triangulation tr;
  // tr.insert(lights.begin(), lights.end());
  // bool survive_til_end=false;
  // for (int i=0; i<m; i++) {
  //   if (CGAL::squared_distance(people[i], tr.nearest_vertex(people[i])->point()) >= (h+radius[i])*(h+radius[i])) {
  //     survive_til_end = true;
  //     cout << i << " ";
  //   }
  // }
  // if (survive_til_end) {
  //   return;
  // }
  
  int low=0, high = n;
  while (low < high) {
    int mid = (low+high+1)/2;
    Triangulation t;
    t.insert(lights.begin(), lights.begin()+mid);
    
    bool survive_til_end=false;
    for (int i=0; i<m; i++) {
      if (CGAL::squared_distance(people[i], t.nearest_vertex(people[i])->point()) >= (h+radius[i])*(h+radius[i])) {
        survive_til_end=true;
        break;
      }
    }
    
    if (survive_til_end) {
      // turn on more lights
      low = mid;
    } else {
      // turn on fewer lights
      high = mid-1;
    }
  }
  
  if (low == 0) {
    for (int i=0; i<m; i++) {
      if (CGAL::squared_distance(people[i], lights[0]) >= (h+radius[i])*(h+radius[i])) {
        cout << i << " ";
      }
    }
  } else {
    Triangulation t;
    t.insert(&lights[0], &lights[low]);
    for (int i=0; i<m; i++) {
      if (CGAL::squared_distance(people[i], t.nearest_vertex(people[i])->point()) >= (h+radius[i])*(h+radius[i])) {
        cout << i << " ";
      }
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