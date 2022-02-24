#include <iostream>
#include <vector>
using namespace std;

void testcase() {
  int n; cin >> n;
  
  vector<int> numbers;
  for (int i=0; i<n; i++) {
    int k; cin >> k;
    numbers.push_back(k);
  }
  
  int e = 0;
  int s = 0;
  for (int i=0; i<n; i++) {
    s += numbers[i];
    if (s % 2 == 0) e++;
  }
  int o = n-e;
  
  int result = e + e*(e-1)/2 + o*(o-1)/2;
  cout << result;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    testcase();
    cout << "\n";
  }
  
  return 0;
}