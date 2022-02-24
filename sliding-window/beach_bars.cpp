///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

void testcase() {
  int n; cin >> n;
  vector<int> p;
  p.reserve(n);
  for (int i=0; i<n; i++) {
    int x; cin >> x;
    p.push_back(x);
  }
  sort(p.begin(), p.end());
  
  int i=0, j=0;
  int count=0;
  int max_n=0;
  int min_d=INT_MAX;
  vector<int> location;
  while (i < n && j < n) {
    if (p[j] - p[i] <= 200) {
      count++;
      // drop decimal points
      int l1 = (p[j] + p[i]) / 2;
      // two best locations if odd range
      int l2 = l1;
      // positive location +1, negative -1
      if ((p[j] + p[i]) % 2) {
        if ((p[j] + p[i]) > 0) {
          l2++;
        } else {
          l2--;
        }
      }
      if (count > max_n) {
        max_n = count;
        min_d = (p[j] - p[i]) / 2;
        if ((p[j] + p[i]) % 2) min_d++;
        location.clear();
        // push in order
        location.push_back(min(l1,l2));
        if (l2 != l1) location.push_back(max(l1,l2));
      } else if (count == max_n) {
        int d = (p[j] - p[i]) / 2;
        if ((p[j] + p[i]) % 2) d++;
        if (d < min_d) {
          min_d = d;
          location.clear();
          location.push_back(min(l1,l2));
          if (l2 != l1) location.push_back(max(l1,l2));
        } else if (d == min_d) {
          location.push_back(min(l1,l2));
          if (l2 != l1) location.push_back(max(l1,l2));
        }
      }
      j++;
    } else {
      i++;
      count--;
    }
  }
  
  cout << max_n << " " << min_d << endl;
  for (const auto l : location) {
    cout << l << " ";
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase();
    cout << "\n";
  }
  return 0;
}