#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

int dfs(vector<int> &times, int n, int i, int t) {
  if (times[i] == -1) return 0;
  if (t >= times[i]) {
    return -1;
  }
    
  if (i >= (n-1)/2) {
    times[i] = -1;
    return 1;
  }
  
  int left = 2*i+1;
  int right = 2*i+2;
  int left_t = dfs(times, n, left, t);
  if (left_t == -1) return -1;
  int right_t = dfs(times, n, right, t);
  if (right_t == -1) return -1;
  
  if (left_t + right_t + 1 + t > times[i]) return -1;
  
  times[i] = -1;
  return left_t + right_t + 1;
}

void testcase() {
  int n; cin >> n;
  vector<pair<int,int>> bomb;
  vector<int> times;
  for (int i=0; i<n; i++) {
    int t; cin >> t;
    bomb.emplace_back(t,i);
    times.push_back(t);
  }
  sort(bomb.begin(), bomb.end());
  
  int total_t = 0;
  for (const auto &b : bomb) {
    if (times[b.second] == -1) continue;
    int t = dfs(times, n, b.second, total_t);
    if (t == -1) {
      cout << "no";
      return;
    }
    total_t += t;
  }
  cout << "yes";
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) {
    testcase(); cout << "\n";
  }
  return 0;
}