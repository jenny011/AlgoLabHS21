#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

int distinct(const int a, const int b, const int c) {
  return (a && a != b && a != c) + (b && b != c) + (c && 1);
}

void testcase() {
  int n,k,m; cin >> n >> k >> m;
  vector<int> fighters;
  fighters.reserve(n);
  for (int i=0; i<n; i++) {
    int t; cin >> t;
    fighters.push_back(t+1);
  }
  
  //2, f1n,f2n, f1s,f2s, diff
  if (m == 3) {
    int memo[2][k+1][k+1][k+1][k+1][25];
    for (int i=0; i<2; i++) {
      for (int n1=0; n1<k+1; n1++) {
        for (int n2=0; n2<k+1; n2++) {
          for (int s1=0; s1<k+1; s1++) {
            for (int s2=0; s2<k+1; s2++) {
              for (int diff = 0; diff < 25; diff++) {
                memo[i][n1][n2][s1][s2][diff] = -1;
              }
            }
          }
        }
      }
    }
    memo[0][fighters[0]][0][0][0][12+1] = 1000-2;
    memo[0][0][0][0][fighters[0]][12-1] = 1000-2;
    int maxe = 998;
    for (int i=1; i<n; i++) {
      int curr = i%2;
      for (int n1=0; n1<k+1; n1++) {
        for (int n2=0; n2<k+1; n2++) {
          for (int s1=0; s1<k+1; s1++) {
            for (int s2=0; s2<k+1; s2++) {
              for (int diff = 0; diff < 25; diff++) {
                memo[curr][n1][n2][s1][s2][diff] = -1;
              }
            }
          }
        }
      }
      
      int prev = 1-curr;
      for (int n1=0; n1<k+1; n1++) {
        for (int n2=0; n2<k+1; n2++) {
          for (int s1=0; s1<k+1; s1++) {
            for (int s2=0; s2<k+1; s2++) {
              for (int diff = 0; diff < 25; diff++) {
                if (memo[prev][n1][n2][s1][s2][diff] != -1) {
                  if (diff + 1 < 25) {
                    int e = 1000*distinct(fighters[i], n1, n2) - (1<<abs(diff-11));
                    if (e >= 0) {
                      memo[curr][fighters[i]][n1][s1][s2][diff+1] = max(memo[curr][fighters[i]][n1][s1][s2][diff+1], memo[prev][n1][n2][s1][s2][diff] + e);
                      maxe = max(memo[curr][fighters[i]][n1][s1][s2][diff+1], maxe);
                    }
                  }
                  if (diff - 1 >= 0) {
                    int e = 1000*distinct(fighters[i], s1, s2) - (1<<abs(diff-13));
                    if (e >= 0) {
                      memo[curr][n1][n2][fighters[i]][s1][diff-1] = max(memo[curr][n1][n2][fighters[i]][s1][diff-1], memo[prev][n1][n2][s1][s2][diff] + e);
                      maxe = max(memo[curr][n1][n2][fighters[i]][s1][diff-1], maxe);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    cout << maxe;
    return;
    
  } else {
    int memo[2][k+1][k+1][25];
    for (int i=0; i<2; i++) {
      for (int n1=0; n1<k+1; n1++) {
        for (int s1=0; s1<k+1; s1++) {
          for (int diff = 0; diff < 25; diff++) {
            memo[i][n1][s1][diff] = -1;
          }
        }
      }
    }
    memo[0][fighters[0]][0][12+1] = 1000-2;
    memo[0][0][fighters[0]][12-1] = 1000-2;
    
    int maxe = 998;
    for (int i=1; i<n; i++) {
      int curr = i%2;
      for (int n1=0; n1<k+1; n1++) {
        for (int s1=0; s1<k+1; s1++) {
          for (int diff = 0; diff < 25; diff++) {
            memo[curr][n1][s1][diff] = -1;
          }
        }
      }
      
      int prev = 1-curr;
      for (int n1=0; n1<k+1; n1++) {
        for (int s1=0; s1<k+1; s1++) {
          for (int diff = 0; diff < 25; diff++) {
            // cout << prev << " " << n1 << " " << s1 << " " << diff << " ";
            // cout << memo[prev][n1][s1][diff] << ",";
            if (memo[prev][n1][s1][diff] != -1) {
              if (diff + 1 < 25) {
                int e = 1000*distinct(fighters[i], n1, 0) - (1<<abs(diff-11));
                if (e >= 0) {
                  memo[curr][fighters[i]][s1][diff+1] = max(memo[curr][fighters[i]][s1][diff+1], memo[prev][n1][s1][diff] + e);
                  maxe = max(memo[curr][fighters[i]][s1][diff+1], maxe);
                }
              }
              if (diff - 1 >= 0) {
                int e = 1000*distinct(fighters[i], s1, 0) - (1<<abs(diff-13));
                if (e >= 0) {
                  memo[curr][n1][fighters[i]][diff-1] = max(memo[curr][n1][fighters[i]][diff-1], memo[prev][n1][s1][diff] + e);
                  maxe = max(memo[curr][n1][fighters[i]][diff-1], maxe);
                }
              }
            }
          }
        }
      }
    }

    cout << maxe;
    return;
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