#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> memo;

int dp(vector<int> &coins, int i, int j, int n) {
  if (i > j) {
    return 0;
  }
  if (memo[i][j] != -1) {
    return memo[i][j];
  }
  
  int c = max(coins[i] + min(dp(coins, i+1, j-1, n), dp(coins, i+2, j, n)),
          coins[j] + min(dp(coins, i, j-2, n), dp(coins, i+1, j-1, n)));
  memo[i][j] = c;
  return c;
}

void testcase() {
  int n; cin >> n;
  vector<int> coins;
  coins.reserve(n);
  for (int i=0; i<n; i++) {
    int v; cin >> v;
    coins.push_back(v);
  }
  
  memo = vector<vector<int>>(n, vector<int>(n, -1));
  int c = dp(coins, 0, n-1, n);
  cout << c;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  
  return 0;
}