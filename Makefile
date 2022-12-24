BOOST_FLAGS=-lboost_coroutine -lboost_context
HEADERS=Graph.hpp StronglyConnectedComponents.hpp Johnson.hpp

libcyclefinder.so: CycleFinder.cpp $(HEADERS)
	$(CXX) -fPIC -shared -O3 CycleFinder.cpp -o $@ $(BOOST_FLAGS)

example: example.c libcyclefinder.so
	$(CC) example.c ./libcyclefinder.so $(BOOST_FLAGS) -o example

clean:
	rm -f *.o *.so example
