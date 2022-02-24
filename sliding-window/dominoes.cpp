#include <iostream>
#include <vector>
using namespace std;

void testcase() {
  int n; cin >> n;
  
  vector<int> domino;
  for (int i=0; i<n; i++) {
    int h; cin >> h;
    domino.push_back(h);
  }
  
  int i=0, j=1;
  int fall = 0;
  while (j <= n) {
    if (j - i >= domino[i] || j == n) {
      // the range can not be extended any more
      // First, record the down dominoes [i, j)
      // Then, stop the list traversal
      fall += j - i;
      break;
    } else {
      // extend the range
      // the new range is defined by j
      // First, record all the down dominoes [i, j)
      // Then, take j as the new start point
      if (domino[j] + j > domino[i] + i) {
        fall += j - i;
        i = j;
      }
      j++;
    }
  }
  
  cout << fall;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  
  return 0;
}