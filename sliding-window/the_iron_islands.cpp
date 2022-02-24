///1
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <climits>
using namespace std;

void testcase() {
  int n,k,w; cin >> n >> k >> w;
  
  vector<int> cost(n);
  for (int i=0; i<n; i++) {
    cin >> cost[i];
  }
  
  // precompute the sum and store in [i][j]
  // avoid offset
  vector<vector<int>> waysum(w);
  vector<int> len(w);
  for (int i=0; i<w; i++) {
    int l; cin >> l;
    len[i] = l;
    for (int j=0; j<l; j++) {
      int r; cin >> r;
      if (j) {
        waysum[i].push_back(waysum[i][j-1] + cost[r]);
      } else {
        waysum[i].push_back(cost[0]);
      }
    }
  }
  
  int maxnum = 0;
  //one way
  for (int i=0; i<w; i++) {
    int a=-1, b=0;
    int men = 0;
    int best = 0;
    int count = 0;
    int l = len[i];
    while (a < l && b <= l) {
      if (a != -1) {
        men = waysum[i][b] - waysum[i][a];
      } else {
        men = waysum[i][b];
      }
      if (men < k && b < l) {
        count++;
        b++;
      } else if (men > k || b == l) {
        if (a != -1) count--;
        a++;
      }
      if (men == k) {
        best = max(best,count);
        if (a != -1) count--;
        a++;
      }
    }
    maxnum = max(maxnum, best);
  }
  
  //two ways
  int augk = k + cost[0];
  // history of all the checked waterways
  unordered_map<int,int> subway;
  // O(W*logK)
  for (int i=0; i<w; i++) {
    vector<int> subcost;
    // compute and check subways
    // way0 pairs up with nothing
    // way1 with way0
    // way2 with way0, way1
    // etc.
    // Go through the waterways and check all pairs in linear time
    for (int j=0; j<len[i]; j++) {
      subcost.push_back(augk - waysum[i][j]);
      if (subway.find(waysum[i][j]) != subway.end()) {
        maxnum = max(maxnum, j + subway[waysum[i][j]]);
      }
    }
    
    // add to history
    for (int j=0; j<len[i]; j++) {
      if (subway.find(subcost[j]) != subway.end()) {
        subway[subcost[j]] = max(subway[subcost[j]], j+1); 
      } else {
        subway[subcost[j]] = j+1; 
      }
    }
  }
  
  cout << maxnum;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}