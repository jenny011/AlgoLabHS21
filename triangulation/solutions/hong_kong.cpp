#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <utility>
#include <queue>
using namespace std;
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Face_handle  Face_handle;
typedef K::Point_2 P;


void testcase() {
  int n,m; K::FT r; cin >> n >> m >> r;
  vector<P> trees;
  trees.reserve(n);
  for (int i=0; i<n; i++) {
    double x,y; cin >> x >> y;
    trees.push_back(P(x,y));
  }
  vector<P> balloons;
  vector<double> radius;
  balloons.reserve(n);
  radius.reserve(n);
  for (int i=0; i<m; i++) {
    double x,y,s; cin >> x >> y >> s;
    balloons.push_back(P(x,y));
    radius.push_back(s);
  }
  
  if (n == 1) {
    for (int i=0; i<m; i++) {
      if (CGAL::squared_distance(balloons[i], trees[0]) >= (r + (K::FT)(radius[i]))*(r + (K::FT)(radius[i]))) {
        cout << "y";
      } else {
        cout << "n";
      }
    }
    return;
  }
  
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); f++) {
    if (t.is_infinite(f)) {
      f->info() = -1;
    } else {
      f->info() = 0;
    }
  }
  
  priority_queue<pair<K::FT, Face_handle>> bfs;
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); f++) {
    if (t.is_infinite(f)) {
      for (int i=0; i<3; i++) {
        Face_handle neighbor = f->neighbor(i);
        if (!t.is_infinite(neighbor)) {
          K::FT gate = CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point());
          bfs.emplace(gate, neighbor);
        }
      }
    } else {
      // connect the finite face to escape with its radius
      K::FT gate = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
      bfs.emplace(gate, f);
    }
  }
  
  while (!bfs.empty()) {
    K::FT new_gate = bfs.top().first;
    Face_handle f = bfs.top().second;
    bfs.pop();
    
    bool check = (f->info() == 0);
    f->info() = max(new_gate, f->info());
    
    if (check) {
      for (int i=0; i<3; i++) {
        Face_handle neighbor = f->neighbor(i);
        if (!t.is_infinite(neighbor)) {
          K::FT gate = CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point());
          bfs.emplace(min(f->info(), gate), neighbor);
        }
      }
    }
  }
  
  for (int i=0; i<m; i++) {
    P p = balloons[i];
    K::FT d = (r + (K::FT)radius[i]) * (r + (K::FT)radius[i]);
    Vertex_handle v = t.nearest_vertex(p);
    // fail at initial position
    if (CGAL::squared_distance(balloons[i], v->point()) < d) {
      cout << "n";
      continue;
    }
    
    Face_handle f = t.locate(p);
    // already escaped, have a path to escape
    if (f->info() == -1 || f->info() >= 4*d) {
      cout << "y";
    } else {
      cout << "n";
    }
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