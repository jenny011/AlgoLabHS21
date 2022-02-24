#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

void testcase(int n) {
  long a,b,c,d; cin >> a >> b >> c >> d;
  R ray(P(a,b), P(c,d));
  
  for (int i = 0; i < n; ++i){
    long r,s,t,u; cin >> r >> s >> t >> u;
    if (CGAL::do_intersect(ray,S(P(r,s), P(t,u)))) {
      cout << "yes";
      for (int j=i+1; j<n; ++j) {
        long r,s,t,u; cin >> r >> s >> t >> u;
      }
      return;
    }
  }
  cout << "no";
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int n; cin >> n;
  while (n) {
    testcase(n); cout << "\n";
    cin >> n;
  }
  
  return 0;
}