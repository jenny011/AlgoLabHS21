#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
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

int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n) {
    int d; cin >> d;
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
    // set the coefficients of A and b
    for (int j=0; j<n; j++) {
      int sum = 0;
      for (int i=0; i<d; i++) {
        int a; cin >> a;
        lp.set_a(i, j, a);
        sum += a*a;
      }
      int b; cin >> b;
      lp.set_a(d, j, sqrt(sum));
      lp.set_b(j, b);
    }
    // set upper bound
    lp.set_l(d, true, 0);       
    // objective function
    lp.set_c(d, -1);        
  
    // solve the program, using ET as the exact type
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
      
    if (s.is_infeasible()) {
      cout << "none";
    } else if (s.is_unbounded()) {
      cout << "inf";
    } else {
      cout << floor_to_double(-s.objective_value());
    }
    cout << "\n";
    cin >> n;
  }
  return 0;
}