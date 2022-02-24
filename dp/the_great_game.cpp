///3
#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
using namespace std;

// r/b, 0/1, n
vector<vector<vector<int>>> steps;

int dp(unordered_map<int, vector<int>> &transitions, int target, int u, int turn, int mp) {
  if (u == target) {
    return 0;
  }
  if (turn == 0) {
    if (steps[mp][turn][u] != -1) return steps[mp][turn][u];
    
    int moves = INT_MAX;
    for (const auto v : transitions[u]) {
      moves = min(moves, 1+dp(transitions, target, v, 1-turn, mp));
    }
    
    steps[mp][turn][u] = moves;
    return moves;
  } else {
    if (steps[mp][turn][u] != -1) return steps[mp][turn][u];
    
    int moves = -1;
    for (const auto v : transitions[u]) {
      moves = max(moves, 1+dp(transitions, target, v, 1-turn, mp));
    }
    
    steps[mp][turn][u] = moves;
    return moves;
  }
}

void testcase(int t) {
  int n,m,r,b; cin >> n >> m >> r >> b;
  unordered_map<int, vector<int>> transitions;
  for (int i=0; i<m; i++) {
    int u,v; cin >> u >> v;
    transitions[u].push_back(v);
  }
  
  steps = vector<vector<vector<int>>>(2, vector<vector<int>>(2, vector<int>(n, -1)));
  // from the perspective of one meeple, 
  // it is always the first one (turn 0) that moves it who wants to minimize the moves
  // and the next one (turn 1) who wants to maximize the moves
  int r_steps = dp(transitions, n, r, 0, 0);
  int b_steps = dp(transitions, n, b, 0, 1);
  
  // this part is tricky
  // experiment with a no jump game: even moves-black meeple takes the last move first
  if (r_steps == b_steps) {
    if (r_steps % 2) {
      cout << 0;
    } else {
      cout << 1;
    }
  } else if (r_steps < b_steps) {
    cout << 0;
  } else {
    cout << 1;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i=0; i<t; i++) {
    testcase(i+1); cout << "\n";
  }
  return 0;
}