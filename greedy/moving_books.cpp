///3
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

void testcase() {
  int n,m; cin >> n >> m;
  vector<int> f(n), b(m);
  for (int i=0; i<n; i++) {
    cin >> f[i];
  }
  sort(f.begin(), f.end(), greater<int>());
  for (int i=0; i<m; i++) {
    cin >> b[i];
  }
  sort(b.begin(), b.end(), greater<int>());
  
  // too few boxes per friend is just not feasible
  // simple loop also works
  int low = m/n; if (low*n < m) low++;
  int high=m+1;
  int middle;
  bool has_solution = false;
  while (low < high) {
    middle = (low+high)/2;
    
    bool feasible = true;
    int fn = m/middle;
    if (fn*middle < m) fn++;
    for (int i=0; i<fn; i++) {
      if (b[i*middle] > f[i]) {
        feasible = false;
        break;
      }
    }
    
    if (feasible) {
      has_solution = true;
      high = middle;
    } else {
      low = middle+1;
    }
  }

  if (has_solution) {
    int t = 3*high - 1;
    cout << t;
  } else {
    cout << "impossible";
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