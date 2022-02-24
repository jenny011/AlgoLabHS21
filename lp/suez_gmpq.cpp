#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <iomanip>
using namespace std;
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase() {
  int n,m; int h,w; cin>>n>>m>>h>>w;
  vector<pair<int,int>> newp,oldp;
  for (int i=0; i<n; i++) {
    int x,y; cin>>x>>y;
    newp.emplace_back(x,y);
  }
  for (int i=0; i<m; i++) {
    int x,y; cin>>x>>y;
    oldp.emplace_back(x,y);
  }
  
  
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  int offset = 0;
  
  for (int i=0; i<n; i++) {
    for (int j=i+1; j<n; j++) {
      if (i == j) continue;
      ET xdiff = CGAL::abs(newp[i].first - newp[j].first);
      ET ydiff = CGAL::abs(newp[i].second - newp[j].second);
      lp.set_a(i, offset, 1); lp.set_a(j, offset, 1); lp.set_b(offset, 2*max(xdiff/w, ydiff/h));
      offset++;
    }
  }
  
  for (int i=0; i<n; i++) {
    ET min_diff = -1;
    for (int j=0; j<m; j++) {
      ET xdiff = CGAL::abs(newp[i].first - oldp[j].first);
      ET ydiff = CGAL::abs(newp[i].second - oldp[j].second);
      if (min_diff == -1) {
        min_diff = max(xdiff/w, ydiff/h);
      } else {
        min_diff = min(min_diff, max(xdiff/w, ydiff/h));
      }
    }
    if (min_diff != -1) {
      lp.set_a(i, offset, 1); lp.set_b(offset, 2*min_diff-1);
      offset++;
    }
  }
  
  for (int i=0; i<n; i++) {
    lp.set_c(i, -2*(w+h));
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  if (!s.is_infeasible()) {
    cout << fixed << setprecision(0) << ceil_to_double(-s.objective_value());
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