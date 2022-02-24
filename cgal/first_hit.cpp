#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n) {
  long a,b,c,d; cin >> a >> b >> c >> d;
  R ray(P(a,b), P(c,d));
  S seg;
  
  vector<S> segs;
  segs.reserve(n);
  for (int i=0; i<n; i++) {
    long r,s,t,u; cin >> r >> s >> t >> u;
    segs.push_back(S(P(r,s), P(t,u)));
  }
  random_shuffle(segs.begin(), segs.end());
  
  bool hit = false;
  P first_hit;
  for (int i = 0; i < n; ++i){
    if (!hit) {
      if (CGAL::do_intersect(ray,segs[i])) {
        auto o = CGAL::intersection(ray, segs[i]);
        if (const P* op = boost::get<P>(&*o)) {
          first_hit = *op;
          seg = S(ray.source(), first_hit);
        } else if (const S* os = boost::get<S>(&*o)) {
          //has_smaller_distance_to_point(P(a,b), src, tgt);
          first_hit = CGAL::min(os->source(), os->target());
          seg = S(ray.source(), first_hit);
        }
        hit = true;
      }
    } else {
      if (CGAL::do_intersect(seg,segs[i])) {
        // construct intersection with ray instead of shortened segment
        auto o = CGAL::intersection(seg, segs[i]);
        if (const P* op = boost::get<P>(&*o)) {
          first_hit = *op;
          seg = S(ray.source(), first_hit);
        } else if (const S* os = boost::get<S>(&*o)) {
          first_hit = CGAL::min(os->source(), os->target());
          seg = S(ray.source(), first_hit);
        }
      }
    }
  }
  
  if (hit) {
    cout << fixed << setprecision(0) << floor_to_double(first_hit.x()) << " " << floor_to_double(first_hit.y());
  } else {
    cout << "no";
  }
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
