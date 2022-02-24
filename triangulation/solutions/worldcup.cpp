///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase() {
  int n,m,c; cin >> n >> m >> c;
  
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  int offset = 0;
  
  vector<P> locations(n+m);
  vector<int> a(n);
  for (int i=0; i<n; i++) {
    int x,y,s,q; cin >> x >> y >> s >> q;
    locations[i] = P(x,y);
    a[i] = q;
    
    // careful with indexing!
    // i*m+j or j*n+i, NEVER multiply by its own total: i*n/j*m WRONG
    for (int j=0; j<m; j++) {
      lp.set_a(i*m+j, offset, 1); 
    }
    lp.set_b(offset, s);
    offset++;
  }
  
  for (int j=0; j<m; j++) {
    int x,y,d,u; cin >> x >> y >> d >> u;
    locations[j+n] = P(x,y);
    
    for (int i=0; i<n; i++) lp.set_a(i*m+j, offset, 1);
    lp.set_b(offset, d);
    offset++;
    
    for (int i=0; i<n; i++) lp.set_a(i*m+j, offset, -1); 
    lp.set_b(offset, -d);
    offset++;
    
    for (int i=0; i<n; i++)  lp.set_a(i*m+j, offset, a[i]); 
    lp.set_b(offset, u*100);
    offset++;
  }
  
  vector<vector<int>> r(n, vector<int>(m));
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      cin >> r[i][j];
    }
  }
  
  vector<P> contour(c);
  vector<long> radius(c);
  for (int i=0; i<c; i++) {
    int x,y; long p; cin >> x >> y >> p;
    contour[i] = P(x,y);
    radius[i] = p*p;
  }
  
  
  Triangulation t;
  t.insert(locations.begin(), locations.end());
  vector<int> non_empty_lines;
  for (int i=0; i<c; i++) {
    if (CGAL::squared_distance(contour[i], t.nearest_vertex(contour[i])->point()) < radius[i]) {
      non_empty_lines.push_back(i);
    }
  }
  
  int non_empty = (int)non_empty_lines.size();
  vector<vector<bool>> wline(n,vector<bool>(non_empty,false)), sline(m,vector<bool>(non_empty,false));
  for (int i=0; i<non_empty; i++) {
    int id = non_empty_lines[i];
    for (int j=0; j<n; j++) {
      wline[j][i] = (CGAL::squared_distance(contour[id], locations[j]) < radius[id]);
    }
    for (int j=0; j<m; j++) {
      sline[j][i] = (CGAL::squared_distance(contour[id], locations[j+n]) < radius[id]);
    }
  }
  
  vector<vector<int>> count(n, vector<int>(m,0));
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      for (int k=0; k<non_empty; k++) {
        if (wline[i][k] != sline[j][k]) count[i][j]++;
      }
    }
  }
  
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      lp.set_c(i*m+j, count[i][j] - 100*r[i][j]);
    }
  }

  // solve the program, using ET as the exact type
  Solution sol = CGAL::solve_linear_program(lp, ET());
  assert(sol.solves_linear_program(lp));

  if (sol.is_infeasible()) {
    cout << "RIOT!";
  } else {
    cout << fixed << setprecision(0) << floor_to_double(-sol.objective_value()/100);
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