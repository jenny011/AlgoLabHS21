#include <iostream>
#include <vector>
using namespace std;

void testcase() {
  int n; cin >> n;
  vector<vector<int>> numbers(n, vector<int>(n,0));
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      cin >> numbers[i][j];
    }
  }
  
  // 2 passes of even pairs on thick rows
  
  int result = 0;
  int e = 0;
  
  // thick rows defined by 0,i
  // compute 0,i,0,j chunks and count even 0,i,j1,j2 chunks
  // 1. correct sums: careful, build the sum
  // 2. count even per i
  vector<vector<int>> sum_row(n,vector<int>(n,0));
  for (int i=0; i<n; i++) {
   sum_row[i][0] = numbers[i][0];
    for (int j=1; j<n; j++) {
      sum_row[i][j] = sum_row[i][j-1] + numbers[i][j];
    }
  }
  for (int i=0; i<n; i++) {
    e = 0;
    if (i==0) {
      for (int j=0; j<n; j++) {
        if (sum_row[i][j] % 2 == 0) e++;
      }
    } else {
      for (int j=0; j<n; j++) {
        sum_row[i][j] = sum_row[i-1][j] + sum_row[i][j];
        if (sum_row[i][j] % 2 == 0) e++;
      }
    }
    result += e + e*(e-1)/2 + (n-e)*(n-e-1)/2;
  }
  
  // thick rows defined by i1,i2
  // compute i1,i2,0,j chunks and count even i1,i2,j1,j2 chunks
  // 1. correct sums: easy, direct subtraction
  // 2. count even per i1,i2
  // vector<vector<vector<int>>> sum_chunk(n,vector<vector<int>>(n,vector<int>(n,0)));
  for (int i=0; i<n; i++) {
    for (int k=i+1; k<n; k++) {
      e = 0;
      for (int j=0; j<n; j++) {
        // sum_chunk[i][k][j] = sum_row[k][j] - sum_row[i][j];
        if ((sum_row[k][j] - sum_row[i][j]) % 2 == 0) e++;
      }
      result += e + e*(e-1)/2 + (n-e)*(n-e-1)/2;
    }
  }
  
  cout << result;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  return 0;
}