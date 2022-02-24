///1
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;

void testcase() {
  int m,n; cin>>m>>n;
  vector<P> hiking;
  hiking.reserve(m);
  for (int i=0; i<m; i++) {
    int x,y; cin>>x>>y;
    hiking.push_back(P(x,y));
  }
  
  vector<vector<bool>> inpoint(n, vector<bool>(m, false));
  for (int i=0; i<n; i++) {
    int x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6;
    cin>>x1>>y1>>x2>>y2>>x3>>y3>>x4>>y4>>x5>>y5>>x6>>y6;
    P p1(x1,y1), p2(x2,y2), p3(x3,y3), p4(x4,y4), p5(x5,y5), p6(x6,y6);
    if (!CGAL::left_turn(p1, p2, p3)) swap(p1,p2);
    if (!CGAL::left_turn(p3, p4, p5)) swap(p3,p4);
    if (!CGAL::left_turn(p5, p6, p1)) swap(p5,p6);

    for (int j=0; j<m; j++) {
      if (CGAL::right_turn(p1, p2, hiking[j])) continue;
      if (CGAL::right_turn(p3, p4, hiking[j])) continue;
      if (CGAL::right_turn(p5, p6, hiking[j])) continue;
      inpoint[i][j] = true;
    }
  }
  
  vector<vector<bool>> include(n, vector<bool>(m-1, false));
  for (int i=0; i<n; i++) {
    for (int j=0; j<m-1; j++) {
      if (inpoint[i][j] && inpoint[i][j+1]) include[i][j] = true;
    }
  }

  int b=0, e=0;
  vector<int> visited(m-1, 0);
  int count = 0;
  int minmaps = n;
  while (b<n && e<=n) {
    if (count == m-1) {
      minmaps = min(minmaps, e-b);
    }
    if (count < m-1 && e < n) {
      for (int j=0; j<m-1; j++) {
        if (include[e][j]) {
          if (!visited[j]) count++;
          visited[j]++;
        }
      }
      e++;
    } else {
      for (int j=0; j<m-1; j++) {
        if (include[b][j]) {
          visited[j]--;
          if (!visited[j]) count--;
        }
      }
      b++;
    }
  }
  
  cout << minmaps;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}