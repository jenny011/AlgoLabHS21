#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

vector<vector<int>> memo;

int dp(vector<int> &coins, int m, int i, int j) {
  if (i > j) {
    return 0;
  }
  
  if (memo[i][j] != -1) {
    return memo[i][j];
  }
  
  int c1 = coins[i];
  int m1 = INT_MAX;
  for (int r=0; r<m; r++) {
    m1 = min(m1, dp(coins, m, i+1+r, j-(m-1-r)));
  }
  int c2 = coins[j];
  int m2 = INT_MAX;
  for (int r=0; r<m; r++) {
    m2 = min(m2, dp(coins, m, i+r, j-1-(m-1-r)));
  }
  
  int max_c = max(c1+m1, c2+m2);
  memo[i][j] = max_c;
  return max_c;
}

void testcase() {
  int n,m,k; cin >> n >> m >> k;
  vector<int> coins;
  for (int i=0; i<n; i++) {
    int v; cin >> v;
    coins.push_back(v);
  }
  
  memo = vector<vector<int>>(n, vector<int>(n, -1));
  int i=0, j=n-1;
  int c = 0;
  int m0 = INT_MAX;
  if (k == 0) {
    m0 = dp(coins, m, i, j);
  } else {
    for (int r=0; r<k+1; r++) {
      m0 = min(m0, dp(coins, m, i+r, j-(k-r)));
    }
  }
  cout << c + m0;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}