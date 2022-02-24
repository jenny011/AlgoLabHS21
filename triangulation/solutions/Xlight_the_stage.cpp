///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>                  Triangulation;
typedef K::Point_2 P;


void testcase() {
  int m,n; cin>>m>>n;
  vector<P> people;
  vector<int> radius;
  people.reserve(m);
  radius.reserve(m);
  for (int i=0; i<m; i++) {
    int x,y,r; cin>>x>>y>>r;
    people.push_back(P(x,y));
    radius.push_back(r);
  }
  
  K::FT h; cin >> h;
  vector<P> lights;
  lights.reserve(n);
  for (int i=0; i<n; i++) {
    int x,y; cin>>x>>y;
    lights.push_back(P(x,y));
  }
  
  vector<int> first_out(m, n);
  Triangulation t;
  t.insert(lights.begin(), lights.end());
  
  bool skip = false;
  for (int i=0; i<m; i++) {
    if (CGAL::squared_distance(people[i], t.nearest_vertex(people[i])->point()) < (h+radius[i])*(h+radius[i])) {
      first_out[i] = n-1;
      if (skip) continue;
      //can't use binary search here
      for (int j=0; j<n; j++) {
        if (CGAL::squared_distance(people[i], lights[j]) < (h+radius[i])*(h+radius[i])) {
          first_out[i] = min(first_out[i], j);
        }
      }
    } else {
      skip = true;
    }
  }
  
  int last_out = *max_element(first_out.begin(), first_out.end());
  for (int i=0; i<m; i++) {
    if (first_out[i] == last_out) cout << i << " ";
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