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
  int p,a,b;
  cin >> p;
  while (p) {
    cin >> a >> b;
    if (p == 1) {
      Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
      // set the coefficients of A and b
      const int X = 0; 
      const int Y = 1;
      lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);  //  x + y  <= 4
      lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  //
      lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);

      // objective function
      lp.set_c(X, a);
      lp.set_c(Y, -b);                                  
    
      // solve the program, using ET as the exact type
      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));
      
      // output solution
      if (s.is_infeasible()) {
        cout << "no";
      } else if (s.is_unbounded()) {
        cout << "unbounded";
      } else {
        cout << floor_to_double(-s.objective_value());
      }
    } else {
      Program lp (CGAL::SMALLER, false, 0, true, 0); 
      
      // set the coefficients of A and b
      const int X = 0; 
      const int Y = 1;
      const int Z = 2;
      lp.set_a(X, 0,-1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);  
      lp.set_a(X, 1,-4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b); 
      lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1); 

      // objective function
      lp.set_c(X, a);                                   
      lp.set_c(Y, b);                                    
      lp.set_c(Z, 1);                              
    
      // solve the program, using ET as the exact type
      Solution s = CGAL::solve_linear_program(lp, ET());
      assert(s.solves_linear_program(lp));
      
      // output solution
      if (s.is_infeasible()) {
        cout << "no";
      } else if (s.is_unbounded()) {
        cout << "unbounded";
      } else {
        cout << ceil_to_double(s.objective_value());
      }
    }
    cout << "\n";
    cin >> p;
  }
  
  return 0;
}
