#ifndef _GRAPHCLASS
#define _GRAPHCLASS

#include <algorithm>
#include <map>
#include <set>
#include <vector>

// Helpers needed in other files
template <class T> bool is_in(const T &item, const std::vector<T> &collection) {
  return std::find(collection.begin(), collection.end(), item) !=
         collection.end();
}

template <class T, class U>
bool is_in(const T &item, const std::map<T, U> &collection) {
  return collection.count(item) > 0;
}

template <class T> bool is_in(const T &item, const std::set<T> &collection) {
  return collection.count(item) > 0;
}

// Digraph
template <class Vertex> class Graph {
public:
  typedef std::pair<Vertex,Vertex> Edge;

  void add_vertex(const Vertex &v);
  void add_edge(const Vertex &v1, const Vertex &v2);
  //void add_edge(const Vertex &v, const std::set<Vertex>& vertices);

  std::set<Vertex> vertices() const;

  //std::set<Edge> edges() const;
  // Returns list of neighbours of vertex
  std::set<Vertex> operator[](const Vertex &v) const;
  // Completely removes a node
  void remove_node(const Vertex &v);

  // Induced subgraph
  Graph<Vertex> subgraph(const std::set<Vertex> &node_set) const;

  // For internal use only:
  //void set_nodes(const std::set<Vertex>& vs) {nodes = vs;}
private:
  std::map<Vertex, std::set<Vertex>> g;
  std::set<Vertex> nodes;
  friend class Graph<Edge>;
};

template <class Vertex> void Graph<Vertex>::add_vertex(const Vertex &v) {
  nodes.insert(v);
}

template <class Vertex>
void Graph<Vertex>::add_edge(const Vertex &v1, const Vertex &v2) {
  g[v1].insert(v2);
  nodes.insert(v1);
  nodes.insert(v2);
}

template <class Vertex> std::set<Vertex> Graph<Vertex>::vertices() const {
  return nodes;
}

template <class Vertex>
std::set<Vertex> Graph<Vertex>::operator[](const Vertex &v) const {
  auto it = g.find(v);
  if (it == g.end()) {
    // Not found
    return std::set<Vertex>();
  } else {
    // Found
    return it->second;
  }
}

// Completely removes a node from the graph
template <class Vertex> void Graph<Vertex>::remove_node(const Vertex &v) {
  g.erase(v);
  for (auto &kv : g) {
    kv.second.erase(v);
  }
  nodes.erase(v);
}

// Intersection of two sets
template <class T>
std::set<T> intersection(const std::set<T> &s1, const std::set<T> &s2) {
  std::set<T> out;
  std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                        std::inserter(out, out.begin()));
  return out;
}

template <class Vertex>
Graph<Vertex> Graph<Vertex>::subgraph(const std::set<Vertex> &node_set) const {
  Graph<Vertex> out;
  auto common_nodes = intersection(nodes, node_set);
  for (const auto &v : common_nodes) {
    out.add_vertex(v);
    auto neighbours = intersection(operator[](v), node_set);
    for (const auto &neighbour : neighbours) {
      out.add_edge(v, neighbour);
    }
  }
  return out;
}

#endif