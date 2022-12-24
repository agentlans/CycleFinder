#ifndef _JOHNSONALGORITHM
#define _JOHNSONALGORITHM

#include <functional>
#include <map>
#include <set>
#include <vector>

#include <boost/coroutine2/all.hpp>
#include <memory>

#include "Graph.hpp"
#include "StronglyConnectedComponents.hpp"

using namespace boost::coroutines2;

template <class T> using Coro = boost::coroutines2::coroutine<T>;

template <class T> T pop(std::set<T> &collection) {
  T x = *collection.begin();
  collection.erase(x);
  return x;
}

template <class T> T pop(std::vector<T> &collection) {
  T x = collection.back();
  collection.pop_back();
  return x;
}

// Returns a generator that returns the simple cycles of directed graph g.
template <class Vertex> auto simple_cycles(const Graph<Vertex> &g) {
  typedef std::set<Vertex> VertexSet;
  typedef std::map<Vertex, VertexSet> VertexDict;
  typedef std::vector<Vertex> Cycle;

  // Return a generator
  return typename Coro<Cycle>::pull_type(
      [&](typename Coro<Cycle>::push_type &sink) {
        // Yield every elementary cycle in graph G exactly once
        auto _unblock = [&](const Vertex &thisnode, VertexSet &blocked,
                            VertexDict &B) {
          VertexSet stack = {thisnode};
          while (!stack.empty()) {
            Vertex node = pop(stack);
            if (is_in(node, blocked)) {
              blocked.erase(node);
              for (const auto &x : B[node]) {
                stack.insert(x);
              }
              B[node].clear();
            }
          }
        };
        // make a copy of the graph
        Graph<Vertex> G = g;
        std::vector<VertexSet> sccs = strongly_connected_components(G);

        while (!sccs.empty()) {
          VertexSet scc = pop(sccs);
          Vertex startnode = pop(scc);
          std::vector<Vertex> path = {startnode};
          VertexSet blocked, closed;
          blocked.insert(startnode);
          VertexDict B;
          std::vector<std::pair<Vertex, VertexSet>> stack;
          stack = {{startnode, G[startnode]}};
          while (!stack.empty()) {
            auto &[thisnode, nbrs] = stack.back();
            if (!nbrs.empty()) {
              Vertex nextnode = pop(nbrs);
              if (nextnode == startnode) {
                sink(path);
                // The above is what Python's yield path[:] does
                for (const auto &x : path) {
                  closed.insert(x);
                }
              } else if (!is_in(nextnode, blocked)) {
                path.push_back(nextnode);
                stack.push_back({nextnode, G[nextnode]});
                closed.erase(nextnode);
                blocked.insert(nextnode);
                continue;
              }
            } else {
              // if not nbrs:
              if (is_in(thisnode, closed)) {
                _unblock(thisnode, blocked, B);
              } else {
                for (const auto &nbr : G[thisnode]) {
                  if (!is_in(thisnode, B[nbr]))
                    B[nbr].insert(thisnode);
                }
              }
              pop(stack);
              pop(path);
            }
          }
          G.remove_node(startnode);
          auto H = G.subgraph(scc);
          for (const auto &comp : strongly_connected_components(H)) {
            sccs.push_back(comp);
          }
        }
      });
}

#endif