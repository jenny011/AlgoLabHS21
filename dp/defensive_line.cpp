///5
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> memo;
vector<int> sum_k;

// DO NOT use the initial value in memo!
// MUST distinguish not visited cases from visited but invalid cases!
// The complexity should be the size of the memo!
int dp(vector<int> &defense, int n, int m, int a, int i) {
  if (a >= m) {
    return 0;
  }
  if (a < m && i >= n) {
    return -2;
  }

  if (memo[a][i] != -1) {
    return memo[a][i];
  }
  
  int max_d = -2;
  int remain = dp(defense, n, m, a, i+1);
  max_d = max(max_d, remain);

  if (sum_k[i] != -1) {
    remain = dp(defense, n, m, a+1, sum_k[i]);
    if (remain != -2) {
      max_d = max(max_d, sum_k[i]-i + remain);
    }
    
  }
  
  memo[a][i] = max_d;
  return max_d;
}

void testcase() {
  int n,m,k; cin >> n >> m >> k;
  vector<int> defense;
  for (int i=0; i<n; i++) {
    int v; cin >> v;
    defense.push_back(v);
  }
  
  memo = vector<vector<int>>(m, vector<int>(n, -1));
  sum_k = vector<int>(n, -1);
  int i=0, j=0;
  int s = 0;
  // tricky sliding window
  // when j reaches n and s > k, not all i are visited
  while (i < n && (j < n || j == n && s > k)) {
    if (s < k && j < n) {
      s += defense[j];
      j++;
    } else if (s >= k) {
      s -= defense[i];
      i++;
    }
    if (s == k) {
      sum_k[i] = j;
    }
  }
  // j=0;
  // int sum=0;
  // for (int i=0; i<n; i++) {
  //   if (i > 0) {
  //     sum -= defense[i-1];
  //   }
  //   while (j < n && sum < k) {
  //     sum += defense[j];
  //     j++;
  //   }
  //   if (sum == k) {
  //     sum_k[i] = j;
  //   }
  // }
  
  int max_d = dp(defense, n, m, 0, 0);
  if (max_d == -2) {
    cout << "fail";
  } else {
    cout << max_d;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while(t--) {
    testcase(); cout << "\n";
  }
  return 0;
}