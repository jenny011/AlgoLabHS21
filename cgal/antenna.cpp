#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  K::Point_2 P;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(int n) {
  vector<P> citizens;
  citizens.reserve(n);
  for (int i=0; i<n; i++) {
    long x,y; cin >> x >> y;
    citizens.push_back(P(x,y));
  }
  Min_circle mc(citizens.begin(), citizens.end(), true);
  Traits::Circle c = mc.circle();
  std::cout << fixed << setprecision(0) << ceil_to_double(CGAL::sqrt(c.squared_radius()));
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