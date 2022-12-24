#ifndef _STRONGLYCONNECTED
#define _STRONGLYCONNECTED

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Graph.hpp"

template <class Vertex>
std::vector<std::set<Vertex>>
strongly_connected_components(const Graph<Vertex> &graph) {
  /*# Tarjan's algorithm for finding SCC's
  # Robert Tarjan. "Depth-first search and linear graph algorithms." SIAM
  journal on computing. 1972. # Based on Python code by Dries Verdegem, November 2012 #
    http://www.logarithmic.net/pfh/blog/01208083168
  */

  typedef std::set<Vertex> VertexList;

  int index_counter = 0;
  std::vector<Vertex> stack;
  std::map<Vertex, int> lowlink;
  std::map<Vertex, int> index;
  std::vector<VertexList> result;

  std::function<void(Vertex)> _strong_connect = [&](Vertex node) -> void {
    index[node] = index_counter;
    lowlink[node] = index_counter;
    index_counter++;
    stack.push_back(node);

    auto successors = graph[node];
    for (const auto &successor : successors) {
      if (!is_in(successor, index)) {
        _strong_connect(successor);

        lowlink[node] = std::min(lowlink[node], lowlink[successor]);
      } else if (is_in(successor, stack)) {
        lowlink[node] = std::min(lowlink[node], index[successor]);
      }
    }

    if (lowlink[node] == index[node]) {
      VertexList connected_component;
      while (true) {
        Vertex successor = stack.back();
        stack.pop_back();
        connected_component.insert(successor);
        if (successor == node)
          break;
      }
      result.push_back(connected_component);
    }
  };

  for (const auto &node : graph.vertices()) {
    if (!is_in(node, index))
      _strong_connect(node);
  }

  return result;
}

#endif