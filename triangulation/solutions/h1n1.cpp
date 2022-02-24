#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <utility>
#include <queue>
using namespace std;
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef Triangulation::Face_handle  Face_handle;
typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;


void testcase(int n) {
  vector<P> infected;
  infected.reserve(n);
  for (int i=0; i<n; i++) {
    int x,y; cin >> x >> y;
    infected.push_back(P(x,y));
  }
  int m; cin >> m;
  vector<P> people;
  vector<K::FT> distance;
  people.reserve(m);
  distance.reserve(m);
  for (int i=0; i<m; i++) {
    int x,y; cin >> x >> y;
    K::FT d; cin >> d;
    people.push_back(P(x,y));
    distance.push_back(d);
  }
  
  if (n==1) {
    for (int i=0; i<m; i++) {
      if (CGAL::squared_distance(people[i], infected[0]) >= distance[i]) {
        cout << "y";
      } else {
        cout << "n";
      }
    }
    return;
  }
  
  Triangulation t;
  t.insert(infected.begin(), infected.end());
  
  // initialize gate to escape value
  // also used as visited tag: 0 unvisited
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    if (t.is_infinite(f)) {
      f->info() = -1;
    } else {
      f->info() = 0;
    }
  }
  
  // populate bfs with all neighbors of infinite faces (escape node)
  priority_queue<pair<K::FT, Face_handle>> bfs;
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    if (t.is_infinite(f)) {
      for (int i=0; i<3; i++) {
        Face_handle neighbor = f->neighbor(i);
        if (!t.is_infinite(neighbor)) {
          K::FT gate = CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point());
          bfs.emplace(gate, neighbor);
        }
      }
    } 
  }
  
  // Update gate to escape value for finite faces
  // check how the neighbors of the current face can go to the current face
  // Push to the queue for each neighbor a gate to escape (min gate on its path to escape): 
  // min(gate from current face to escape, gate from neighbor to current face)
  // For the current face, update the gate to escape: max(poped gate, saved gate)
  while (!bfs.empty()) {
    K::FT new_gate = bfs.top().first;
    Face_handle f = bfs.top().second;
    bfs.pop();
    
    bool check = (f->info() == 0);
    f->info() = max(f->info(), new_gate);
    
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
    P p = people[i];
    K::FT d = distance[i];
    Vertex_handle v = t.nearest_vertex(p);
    if (CGAL::squared_distance(v->point(),p) < d) {
      cout << "n";
      continue;
    }
    Face_handle f = t.locate(p);
    if (t.is_infinite(f) || f->info() >= 4*d) {
      cout << "y";
    } else {
      cout << "n";
    }
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