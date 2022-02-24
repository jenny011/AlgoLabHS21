#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <climits>
using namespace std;

typedef unordered_map<string,int> Strmap;
typedef unordered_map<int,vector<int>> Tree;
typedef unordered_map<int,vector<pair<int,int>>> Query;

vector<int> results;
vector<pair<int,int>> path;

void dfs(Query &query, Tree &relation, vector<int> &age, int i) {
  path.emplace_back(age[i], i);
  for (const auto &q : query[i]) {
    int low=0, high=(int)path.size();
    while (low < high) {
      int mid = (low+high)/2;
      if (path[mid].first <= q.first) {
        high = mid;
      } else {
        low = mid+1;
      }
    }
    results[q.second] = path[low].second;
  }
  
  for (const auto child : relation[i]) {
    dfs(query, relation, age, child);
  }
  path.pop_back();
  return;
}

void testcase() {
  int n,q; cin >> n >> q;
  Strmap nameid;
  vector<string> names(n);
  vector<int> age(n);
  for (int i=0; i<n; i++) {
    string s; cin >> s;
    names[i] = s;
    nameid[s] = i;
    cin >> age[i];
  }
  
  Tree relation;
  vector<bool> is_root(n, true);
  for (int i=0; i<n-1; i++) {
    string s,p; cin >> s >> p;
    relation[nameid[p]].push_back(nameid[s]);
    is_root[nameid[s]] = false;
  }
  
  int root;
  for (int i=0; i<n; i++) {
    if (is_root[i]) {
      root = i;
      break;
    }
  }
  
  Query query;
  for (int i=0; i<q; i++) {
    string s; int a; cin >> s >> a;
    query[nameid[s]].emplace_back(a,i);
  }
  
  results = vector<int>(q, -1);
  dfs(query, relation, age, root);
  
  for (const int ret : results) {
    cout << names[ret] << " ";
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