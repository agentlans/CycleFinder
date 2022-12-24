#include <vector>
#include <memory>

#include "Graph.hpp"
#include "Johnson.hpp"

// C header
#include "CycleFinder.h"

// Wrapper for the graph and for the coroutine to generate cycles
class CycleFinder {
public:
    typedef std::vector<int> Cycle;
    typedef typename Coro<Cycle>::pull_type Generator;
    // Add directed edges to the graph
    void add(int i, int j) {
        g.add_edge(i,j);
    }
    // Set up the graph for the generator
    void set() {
        generator = std::make_unique<Generator>(simple_cycles(g));
    }
    // Reset to empty graph
    void clear() {
        g = Graph<int>();
        cycle.clear();
    }
    // Runs the generator
    void find_cycle() {
        cycle = generator->get();
        (*generator)();
    }
    // Returns pointer to the found cycle. Note: must run find_cycle() first.
    int* get_cycle(size_t* size) {
        *size = cycle.size();
        return cycle.data();
    }
    // Whether the generator has found every cycle
    bool finished() const {
        return !(generator->operator bool());
    }
private:
    Graph<int> g;
    Cycle cycle;
    std::unique_ptr<Generator> generator;
};

// C definitions

#define OBJ ((CycleFinder*) obj)

cycf cycf_new() {
    return new CycleFinder();
}

void cycf_free(cycf obj) {
    delete OBJ;
}

void cycf_add(cycf obj, int i, int j) {
    OBJ->add(i,j);
}

void cycf_set(cycf obj) {
    OBJ->set();
}

void cycf_clear(cycf obj) {
    OBJ->clear();
}

void cycf_find_cycle(cycf obj) {
    OBJ->find_cycle();
}

int* cycf_get_cycle(cycf obj, size_t* size) {
    return OBJ->get_cycle(size);
}

int cycf_finished(cycf obj) {
    return OBJ->finished();
}
