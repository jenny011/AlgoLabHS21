///
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
typedef pair<long,long> pll;

bool backtrack(vector<pair<long,long>> moves, vector<long> &remain, int count, int n, int i, long d_remain, long t_remain) {
  if (d_remain <= 0 && t_remain > 0) {
    return true;
  }
  if (t_remain <= 0) {
    return false;
  }
  if (i == n) {
    if (count) remain[count] = min(remain[count], d_remain);
    return false;
  }

  return backtrack(moves, remain, count, n, i+1, d_remain, t_remain)
      || backtrack(moves, remain, count+1, n, i+1, d_remain-moves[i].first, t_remain-moves[i].second);
}

void testcase() {
  int n,m;
  long D,T;
  cin>>n>>m>>D>>T;
  vector<pair<long,long>> moves;
  for (int i=0; i<n; i++) {
    long d,t; cin >> d >> t;
    moves.emplace_back(d,t);
  }
  
  vector<long> potion;
  for (int i=0; i<m; i++) {
    long s; cin >> s;
    potion.push_back(s);
  }
  
  vector<long> remain(n+1, D);
  bool success = backtrack(moves, remain, 0, n, 0, D, T);
  
  if (success) {
    cout << 0;
    return;
  }
  
  int low = 0, high = m;
  while (low < high) {
    int mid = (low+high)/2;
    bool ok = false;
    for (int i=1; i<n+1; i++) {
      if (remain[i] < D && remain[i] - i*potion[mid] <= 0) {
        high = mid;
        ok = true;
        continue;
      }
    }
    if (!ok) low = mid+1;
  }
  
  if (high == m) {
    cout << "Panoramix captured";
  } else {
    cout << high+1;
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