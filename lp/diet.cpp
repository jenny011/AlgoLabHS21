#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}


double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

int main() {
  ios_base::sync_with_stdio(false);
  int n,m; cin >> n >> m;
  while (n && m) {
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    
    for (int i=0; i<n; i++) {
      int a,b; cin >> a >> b;
      lp.set_b(i, -a);
      lp.set_b(i+n, b);
    }
    
    for (int j=0; j<m; j++) {
      int cost; cin >> cost;
      lp.set_c(j, cost);
      for (int i=0; i<n; i++) {
        int a; cin >> a;
        lp.set_a(j, i, -a);
        lp.set_a(j, i+n, a);
      }
    } 
  
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    if (s.is_infeasible()) {
      cout << "No such diet.";
    } else {
      cout << floor_to_double(s.objective_value());
    }
    cout << "\n";
    
    cin >> n >> m;
  }
  return 0;
}