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
VPLL moves;


bool backtrack(VPLL &moves, VPLL &used, int i, long d, long t, int p) {
  if (t >= T) return false;
  if (d >= D && t < T) return true;
  if (i == (int)moves.size()) {
    return false;
  }

  return backtrack(moves, used, i+1, d, t, p)
      || backtrack(moves, used, i+1, d+moves[i].first+potion[p], t+moves[i].second, p);
}

bool tryit(int p) {
  // don't care about the count, it will always be < n !!!!
  // find a pair that succeeds
  VPLL used;
  bool success = backtrack(moves, used, 0, 0, 0, p); 
  if (success) return true; 
  return false;
}

void testcase() {
  int n,m;
  cin>>n>>m>>D>>T;
  
  moves = VPLL(n);
  long d,t; 
  for (int i=0; i<n; i++) {
    cin >> d >> t;
    moves[i] = {d,t};
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