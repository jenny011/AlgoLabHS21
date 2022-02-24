///3
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef pair<int,int> P;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase() {
  int n,m; long sew; cin >> n >> m >> sew;

  vector<P> noble, common;
  for (int i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    noble.emplace_back(x,y);
  }
  for (int i=0; i<m; i++) {
    int x,y; cin >> x >> y;
    common.emplace_back(x,y);
  }
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  
  // set the coefficients of A and b
  const int A = 0; 
  const int B = 1;
  const int D = 2;
  const int L = 3;
  
  int offset = 0;
  for (int i=0; i<n; i++) {
    lp.set_a(A, offset, noble[i].second); lp.set_a(D, offset, -1); lp.set_b(offset, -noble[i].first);
    offset++;
  }
  for (int i=0; i<m; i++) {
    lp.set_a(A, offset, -common[i].second); lp.set_a(D, offset, 1); lp.set_b(offset, common[i].first);
    offset++;
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  if (s.is_infeasible()) {
    cout << "Yuck!";
  } else {
    for (int i=0; i<n; i++) {
      lp.set_a(A, offset, noble[i].first); lp.set_a(B, offset, 1); lp.set_a(L, offset, -1); lp.set_b(offset, noble[i].second);
      offset++;
      lp.set_a(A, offset, -noble[i].first); lp.set_a(B, offset, -1); lp.set_a(L, offset, -1); lp.set_b(offset, -noble[i].second);
      offset++;
    }
    for (int i=0; i<m; i++) {
      lp.set_a(A, offset, common[i].first); lp.set_a(B, offset, 1); lp.set_a(L, offset, -1); lp.set_b(offset, common[i].second);
      offset++;
      lp.set_a(A, offset, -common[i].first); lp.set_a(B, offset, -1); lp.set_a(L, offset, -1); lp.set_b(offset, -common[i].second);
      offset++;
    }
    
    if (sew != -1) {
      long sumx=0, sumy=0, sum1=0;
      for (int i=0; i<n; i++) {
        sumx -= noble[i].first;
        sumy -= noble[i].second;
        sum1++;
      }
      for (int i=0; i<m; i++) {
        sumx += common[i].first;
        sumy += common[i].second;
        sum1--;
      }
      lp.set_a(A, offset, sumy); lp.set_a(D, offset, sum1); lp.set_b(offset, sew-sumx);
    }
    
    lp.set_l(L, true, 1);  
    // objective function
    lp.set_c(L, 1);                                  
  
    // solve the program, using ET as the exact type
    s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    
    if (s.is_infeasible()) {
      cout << "Bankrupt!";
    } else {
      cout << fixed << setprecision(0) << ceil_to_double(s.objective_value());
    }
  }
}

int main()
{
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  while (t--) {
    testcase(); cout << "\n";
  }
  
  return 0;
}
