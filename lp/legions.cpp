#include <iostream>
#include <vector>
#include <algorithm>
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


double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase() {
  long x,y,n; cin >> x >> y >> n;
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  const int T=0;
  const int X=1;
  const int Y=2;
  for (int i=0; i<n; i++) {
    int a,b,c,v; cin >> a >> b >> c >> v;
    int norm = sqrt((long)a*a + (long)b*b);
    if (a*x + b*y + c < 0) {
      lp.set_a(T, i, v*norm); lp.set_a(X, i, a);  lp.set_a(Y, i, b); lp.set_b(i, -c);
    } else {
      lp.set_a(T, i, v*norm); lp.set_a(X, i, -a);  lp.set_a(Y, i, -b); lp.set_b(i, c);
    }
  }
  
  lp.set_l(T, true, 1);                        
  // objective function
  lp.set_c(T, -1);                                

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  if (s.is_infeasible()) {
    cout << "no";
  } else {
    cout << fixed << setprecision(0) << floor_to_double(-s.objective_value());
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