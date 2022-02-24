#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <climits>
#include <cmath>
using namespace std;
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef pair<int,int> P;

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase() {
  int n,m; long h,w; cin>>n>>m>>h>>w;
  
  vector<P> newp, oldp;
  for (int i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    newp.emplace_back(x,y);
  }
  for (int i=0; i<m; i++) {
    int x,y; cin >> x >> y;
    oldp.emplace_back(x,y);
  }
  
  vector<int> min_x_diff(n,-1), min_y_diff(n,-1);
  for (int i=0; i<n; i++) {
    int x = newp[i].first, y = newp[i].second;
    for (int j=0; j<m; j++) {
      int a = oldp[j].first, b = oldp[j].second;
      int dx = abs(x-a), dy = abs(y-b);
      if (w*dy < h*dx) {
        //x: < h/w
        if (min_x_diff[i] == -1) {
          min_x_diff[i] = 2*dx;
        } else {
          min_x_diff[i] = min(min_x_diff[i], 2*dx);
        }
      } else {
        //y
        if (min_y_diff[i] == -1) {
          min_y_diff[i] = 2*dy;
        } else {
          min_y_diff[i] = min(min_y_diff[i], 2*dy);
        }
      }
    }
  }
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  
  int offset=0;
  for (int i=0; i<n; i++) {
    if (min_x_diff[i] != -1) {
      lp.set_a(i, offset, w); lp.set_b(offset, min_x_diff[i]-w);
      offset++;
    }
    if (min_y_diff[i] != -1){
      lp.set_a(i, offset, h); lp.set_b(offset, min_y_diff[i]-h);
      offset++;
    }
    // objective function
    lp.set_c(i, -2*(h+w));     
  }
  
  for (int i=0; i<n; i++) {
    int x = newp[i].first, y = newp[i].second;
    for (int j=0; j<n; j++) {
      if (j == i) continue;
      int a = newp[j].first, b = newp[j].second;
      int dx = abs(x-a), dy = abs(y-b);
      if (w*dy < h*dx) {
        //x: < h/w
        lp.set_a(i, offset, w); lp.set_a(j, offset, w); lp.set_b(offset, 2*dx);
        offset++;
      } else {
        //y
        lp.set_a(i, offset, h); lp.set_a(j, offset, h); lp.set_b(offset, 2*dy);
        offset++;      
      }
    }
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  if (s.is_infeasible()) {
    cout << "no";
  } else {
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