///
#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
using namespace std;

void testcase() {
  int n, k; cin >> n >> k;
  vector<int> cards;
  for (int i=0; i<n; i++) {
    int x; cin >> x;
    cards.push_back(x);
  }
  
  
  int i=0, j=0;
  int s = cards[0];
  int best_diff = INT_MAX;
  int best_i=0, best_j=n-1;
  // two loops vs. one loop
  // one round of phases at a time
  while (i < n && j < n) {
    //sliding window phase 1: increment j, grow the sum until >=k
    while (s < k && j < n) {
      // compare as you sum, time permits, fewer corner cases, easier to code
      // check then grow: same as while loop check
      if (k-s < best_diff) {
        best_i = i;
        best_j = j;
        best_diff = k-s;
      }
      j++;
      s += cards[j];
    }
    if (s == k) {
      cout << i << " " << j;
      return;
    }
    //sliding window phase 2: increment i, shrink the sum until <=k
    while (s > k && i < n) {
      if (s-k < best_diff) {
        best_i = i;
        best_j = j;
        best_diff = s-k;
      }
      s -= cards[i];
      i++;
    }
    if (s == k) {
      cout << i << " " << j;
      return;
    }
  }
  
  // one increment at a time
  // while (i < n && j < n) {
  //   if (s < k) {
  //   // sliding window phase 1: if <k, increment j, grow the sum
  //     j++;
  //     s += cards[j];
  //   }
  //   else if (s > k) {
  //   // sliding window phase 2: if >k, increment i, shrink the sum until
  //     s -= cards[i];
  //     i++;
  //   } else {
  //     cout << i << " " << j;
  //     return;
  //   }

  //   if (abs(s-k) < best_diff) {
  //     best_i = i;
  //     best_j = j;
  //     best_diff = abs(s-k);
  //   }
  // }
  
  cout << best_i << " " << best_j;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}
