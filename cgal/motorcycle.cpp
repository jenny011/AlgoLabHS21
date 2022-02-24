///4
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
using namespace std;
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

void testcase() {
  int n; cin >> n;
  vector<pair<long,int>> start;
  vector<K::FT> slope;
  for (int i=0; i<n; i++) {
    long a,b,c; cin>>a>>b>>c;
    start.emplace_back(a, i);
    
    if (!b) {
      slope.push_back(0);
    } else {
      K::FT k = (K::FT)(c-a)/(K::FT)b;
      slope.push_back(k);
    }
  }
  sort(start.begin(), start.end());
  
  vector<bool> forever(n, true);
  K::FT min_slope = slope[start[n-1].second];
  for (int i=n-2; i>=0; i--) {
    int id = start[i].second;
    if (CGAL::abs(slope[id]) <= CGAL::abs(min_slope)) {
      min_slope = slope[id];
    } else if (slope[id] > min_slope) {
      forever[id] = false;
    }
  }
  
  min_slope = slope[start[0].second];
  for (int i=1; i<n; i++) {
    int id = start[i].second;
    if (CGAL::abs(slope[id]) < CGAL::abs(min_slope) || (slope[id] == -min_slope && slope[id] > 0)) {
      min_slope = slope[id];
    } else if (slope[id] < min_slope) {
      forever[id] = false;
    }
  }
  
  for (int i=0; i<n; i++) {
    if (forever[i]) cout << i << " ";
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