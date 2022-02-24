///3
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

void testcase() {
  int n; cin >> n;
  vector<pair<int,int>> rb;
  for (int i=0; i<n; i++) {
    int l,p; cin >> l >> p;
    rb.emplace_back(p,l);
  }
  sort(rb.begin(), rb.end());
  
  int i=0, j=1;
  int back=rb[0].first;
  int new_back = rb[0].first;
  int count=0;
  while (j < n && i < n) {
    if (rb[j].first >= new_back) {
      // the first interval since back that can be non-overlapping
      // All visited rings between it and back have to overlap with the so far best new_back
      // while new_back is already made optimal (earliest) given all those visited rings
      // so, all those visited rings count as one boat
      back = new_back;
      new_back = INT_MAX;
      count++;
    }
    // calculate my optimal(earliest) back based on back, update the new_back if my_back is better
    if (rb[j].first - back < rb[j].second) {
      int my_back = back + rb[j].second;
      if (my_back < new_back) {
        new_back = my_back;
        i = j;
      }
    } else {
      if (rb[j].first < new_back) {
        new_back = rb[j].first;
        i = j;
      }
    }
    j++;
  }
  if (rb[j].first < new_back) {
    count++;
  }
  cout << count;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}