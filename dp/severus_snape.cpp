///1
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <climits>
using namespace std;

typedef pair<long, int> PH;
 
void testcase() {
  int n,m; 
  long a,b,p,w; int h; 
  cin >> n >> m >> a >> b >> p >> h >> w;
  
  vector<PH> ph;
  for (int i=0; i<n; i++) {
    long pv; int hv; cin >> pv >> hv;
    ph.emplace_back(pv, hv);
  }
  
  vector<long> wit;
  for (int i=0; i<m; i++) {
    long wv; cin >> wv;
    wit.push_back(wv);
  }
  
  sort(ph.begin(), ph.end(), greater<PH>());
  sort(wit.begin(), wit.end(), greater<long>());
  
  vector<vector<vector<long>>> memo(2, vector<vector<long>>(n+1, vector<long>(h+1, LONG_MAX)));
  memo[0][1][max(0,h-ph[0].second)] = p-ph[0].first;
  memo[0][0][h] = p;
  for (int i=1; i<n; i++) {
    memo[i%2] = vector<vector<long>>(n+1, vector<long>(h+1, LONG_MAX));
    for (int j=0; j<n; j++) {
      for (int k=0; k<h+1; k++) {
        if (memo[1-i%2][j][k] != LONG_MAX) {
          memo[i%2][j][k] = min(memo[i%2][j][k], memo[1-i%2][j][k]);
          memo[i%2][j+1][max(0,k-ph[i].second)] = min(memo[i%2][j+1][max(0,k-ph[i].second)], memo[1-i%2][j][k] - ph[i].first);
        }
      }
    }
  }
  
  int result = INT_MAX;
  int j;
  long new_w;
  for (int i=0; i<n+1; i++) {
    if (memo[1-n%2][i][0] <= 0) {
      new_w = w + a*i;
      j = 0;
      while (new_w > 0 && j < m) {
        new_w -= wit[j];
        j++;
      }
      if (new_w <= 0 && b*j + memo[1-n%2][i][0] <= 0) {
        result = min(result, i + j);
      }
    }
  }
  
  if (result == INT_MAX) {
    cout << -1;
  } else {
    cout << result;
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