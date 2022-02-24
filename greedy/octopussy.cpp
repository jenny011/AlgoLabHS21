///1
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

long tree(vector<long> &explode, vector<bool> &deact, int i, long t) {
  int left = 2*i+1;
  int right = left+1;
  
  long lt = 0, rt = 0;
  if (i < ((int)explode.size()-1)/2) {
    if (explode[left] < explode[right]) {
      if (!deact[left]) {
        lt = tree(explode, deact, left, t);
        if (lt == -1) {
          return -1;
        }
      }
      if (!deact[right]) {
        rt = tree(explode, deact, right, t+lt);
        if (rt == -1) {
          return -1;
        }
      }
    } else {
      if (!deact[right]) {
        rt = tree(explode, deact, right, t);
        if (rt == -1) {
          return -1;
        }
      }
      if (!deact[left]) {
        lt = tree(explode, deact, left, t+rt);
        if (lt == -1) {
          return -1;
        }
      }
    }
  }
  if (t + lt + rt + 1 <= explode[i]) {
    deact[i] = true;
    return lt + rt + 1;
  } else {
    return -1;
  }
}

void testcase(){
  int n; cin >> n;
  vector<long> explode;
  vector<pair<long,int>> bombs;
  for (int i=0; i<n; i++) {
    long t; cin >> t;
    explode.push_back(t);
    bombs.emplace_back(t,i);
  }
  sort(bombs.begin(), bombs.end());
  vector<bool> deact(n, false);
  
  long timer = 0;
  for (int i=0; i<n; i++) {
    if (deact[bombs[i].second]) continue;
    if (timer > bombs[i].first) {
      cout << "no";
      return;
    }
    long total_t = tree(explode, deact, bombs[i].second, timer);
    if (total_t == -1) {
      cout << "no";
      return;
    }
    timer += total_t;
  }
  cout << "yes";
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}