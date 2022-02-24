#include <iostream>
using namespace std;

void testcase() {
  int n; cin >> n;
  int s = 0;
  for (int i=0; i<n; i++) {
    int k; cin >> k;
    s += k;
  }
  cout << s;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  
  return 0;
}