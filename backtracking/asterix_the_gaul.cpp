///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <utility>
using namespace std;
typedef vector<long> VL;
typedef vector<pair<long,long>> VPLL;

long D,T;
VL potion;
VPLL moves1, moves2;


bool backtrack(VPLL &moves, VPLL &used, int i, long d, long t, int p) {
  if (t >= T) return false;
  if (d >= D && t < T) return true;
  if (i == (int)moves.size()) {
    if (d > 0) used.emplace_back(t, d);
    return false;
  }

  return backtrack(moves, used, i+1, d, t, p)
      || backtrack(moves, used, i+1, d+moves[i].first+potion[p], t+moves[i].second, p);
}

bool tryit(int p) {
  // don't care about the count, it will always be < n !!!!
  // find a pair that succeeds
  VPLL used1, used2;
  bool success = backtrack(moves1, used1, 0, 0, 0, p); 
  if (success) return true;
  success = backtrack(moves2, used2, 0, 0, 0, p);
  if (success) return true;
  
  sort(used1.begin(), used1.end());
  sort(used2.begin(), used2.end());
  for (auto it=++used1.begin(); it!=used1.end();it++) {
    it->second = max((it-1)->second, it->second);
  }
  for (auto it=++used2.begin(); it!=used2.end();it++) {
    it->second = max((it-1)->second, it->second);
  }
  
  for (auto it=used1.cbegin(); it!=used1.cend(); it++) {
    int low = 0, high = (int)used2.size();
    while (low < high) {
      int mid = low + (high+low+1)/2;
      if (it->first + used2[mid].first < T) {
        if (it->second + used2[mid].second >= D) {
          return true;
        }
        low = mid+1;
      } else {
        high = mid;
      }
    }
  } 
  return false;
}

void testcase() {
  int n,m;
  cin>>n>>m>>D>>T;
  
  int split = n/2;
  moves1 = VPLL(split);
  long d,t; 
  for (int i=0; i<split; i++) {
    cin >> d >> t;
    moves1[i] = {d,t};
  }
  
  moves2 = VPLL(n-split);
  for (int i=0; i<n-split; i++) {
    cin >> d >> t;
    moves2[i] = {d,t};
  }
  
  potion = VL(m+1);
  potion[0] = 0;
  for (int i=1; i<m+1; i++) {
    cin >> potion[i];
  }
  
  int low = 0, high = m+1;
  while (low < high) {
    int mid = (low+high)/2;
    if (tryit(mid)) {
      high = mid;
    } else {
      low = mid+1;
    }
  }
  
  if (high == m+1) {
    cout << "Panoramix captured";
  } else  {
    cout << high;
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