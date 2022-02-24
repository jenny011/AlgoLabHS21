#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

void testcase() {
  int n,k; cin >> n >> k;
  vector<int> cost(n), volume(n);
  for (int i=0; i<n; i++) {
    cin >> cost[i];
    cin >> volume[i];
  }
  
  // k+1: 0 (include negative) to k volume
  vector<vector<pair<int,int>>> dp(n, vector<pair<int,int>>(k+1, pair<int,int>(-1,-1)));
  // no bottle is needed
  dp[0][0] = pair<int,int>(0,0);
  for (int j=1; j<k+1; j++) {
    // over the limit is allowed: negative remaining volume ==> 0
    dp[0][j].first = dp[0][max(0,j-volume[0])].first + cost[0]; 
    dp[0][j].second = 1;
  }
  
  for (int i=1; i<n; i++) {
    for (int j=0; j<k+1; j++) {
      int c1 = dp[i-1][j].first;
      int d1 = dp[i-1][j].second;
      int c2 = dp[i-1][max(0,j-volume[i])].first+cost[i];
      int d2 = dp[i-1][max(0,j-volume[i])].second+1;
      if (c1 < c2 || (c1 == c2 && d1 > d2)) {
        //c1 best
        dp[i][j] = dp[i-1][j];
      } else {
        //c2 best
        dp[i][j] = pair<int,int>(c2,d2);
      }
      int c = dp[i][j].first;
      int d = dp[i][j].second;
      int c3 = dp[i][max(0,j-volume[i])].first+cost[i];
      int d3 = dp[i][max(0,j-volume[i])].second;
      if (c3 < c || (c3 == c && d3 > d)) {
        //c3 best
        dp[i][j] = pair<int,int>(c3,d3);
      }
    }
  }
  
  cout << dp[n-1][k].first << " " << dp[n-1][k].second;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}