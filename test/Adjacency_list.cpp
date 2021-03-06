
#include <graph/Adjacency_list.hpp>

#include "Graph_tester.hpp"

SCENARIO("out-adjacency lists behave properly", "[Out_adjacency_list]") {
	using G = graph::Out_adjacency_list;
	GIVEN("an empty graph") {
		G g;
		REQUIRE(g.order() == 0);
		REQUIRE(g.size() == 0);
		Out_edge_graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("a self-edge is inserted and its tail is erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_vert(v);
			REQUIRE(g.order() == 0);
			REQUIRE(g.size() == 0);
		}
		WHEN("an edge is inserted and its tail is erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_vert(s);
			REQUIRE(g.order() == 1);
			REQUIRE(g.size() == 0);
		}
		WHEN("the graph is cleared") {
			gt.clear();
		}
	}
	GIVEN("a random graph") {
		std::mt19937 r;
		G g;
		const std::size_t M = 20, N = 100;
		for (std::size_t m = 0; m < M; ++m)
			g.insert_vert();
		for (std::size_t n = 0; n < N; ++n) {
			auto s = g.random_vert(r), t = g.random_vert(r);
			g.insert_edge(s, t);
		}
		REQUIRE(g.order() == M);
		REQUIRE(g.size() == N);
		Out_edge_graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("all the edges are erased") {
			while (g.size())
				gt.erase_edge(gt.random_edge(r));
		}
		WHEN("viewed in reverse") {
			assert(g.size()); // sanity check on the test itself
			auto rg = g.reverse_view();
			In_edge_graph_tester rgt{rg};
		}
		WHEN("searching for shortest paths from a vertex") {
			auto s = gt.random_vert(r);
			//auto weight = [](auto e) { return 1; };
			auto weight = g.edge_map(0.0);
			for (auto e : g.edges())
				weight[e] = std::uniform_real_distribution<double>{}(r);
			auto [tree, distances] = g.shortest_paths_from(s, weight);
			REQUIRE(tree.root() == s);
			REQUIRE(distances(s) == 0);
			for (auto v : g.verts()) {
				auto e = tree.in_edge_or_null(v);
				if (e != g.null_edge()) {
					REQUIRE(g.head(e) == v);
					REQUIRE(distances(v) == distances(g.tail(e)) + weight(e));
				}
			}
			for (auto e : g.edges())
				REQUIRE(!(distances(g.head(e)) > distances(g.tail(e)) + weight(e)));
		}
		WHEN("searching for the minimum spanning tree") {
			auto s = gt.random_vert(r);
			auto tree = g.minimum_tree_reachable_from(s, [](auto e) { return 1; });
			REQUIRE(tree.root() == s);
			// If the tail of an edge is in the tree, then so must be the head
			for (auto e : g.edges())
				REQUIRE((!tree.in_tree(g.tail(e)) || tree.in_tree(g.head(e))));
			// TODO: Use the cut lemma to verify this tree has minimal weight
		}
	}
}

SCENARIO("in-adjacency lists behave properly", "[In_adjacency_list]") {
	using G = graph::In_adjacency_list;
	GIVEN("an empty graph") {
		G g;
		REQUIRE(g.order() == 0);
		REQUIRE(g.size() == 0);
		In_edge_graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("a self-edge is inserted and its head is erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_vert(v);
			REQUIRE(g.order() == 0);
			REQUIRE(g.size() == 0);
		}
		WHEN("an edge is inserted and its head is erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_vert(t);
			REQUIRE(g.order() == 1);
			REQUIRE(g.size() == 0);
		}
		WHEN("the graph is cleared") {
			gt.clear();
		}
	}
	GIVEN("a random graph") {
		std::mt19937 r;
		G g;
		const std::size_t M = 20, N = 100;
		for (std::size_t m = 0; m < M; ++m)
			g.insert_vert();
		for (std::size_t n = 0; n < N; ++n) {
			auto s = g.random_vert(r), t = g.random_vert(r);
			g.insert_edge(s, t);
		}
		REQUIRE(g.order() == M);
		REQUIRE(g.size() == N);
		In_edge_graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("all the edges are erased") {
			while (g.size())
				gt.erase_edge(gt.random_edge(r));
		}
		WHEN("the graph is cleared") {
			gt.clear();
		}
		WHEN("viewed in reverse") {
			assert(g.size()); // sanity check on the test itself
			auto rg = g.reverse_view();
			Out_edge_graph_tester rgt{rg};
		}
		WHEN("searching for shortest paths to a vertex") {
			auto t = gt.random_vert(r);
			//auto weight = [](auto e) { return 1; };
			auto weight = g.edge_map(0.0);
			for (auto e : g.edges())
				weight[e] = std::uniform_real_distribution<double>{}(r);
			auto [tree, distances] = g.shortest_paths_to(t, weight);
			REQUIRE(tree.root() == t);
			REQUIRE(distances(t) == 0);
			for (auto v : g.verts()) {
				auto e = tree.out_edge_or_null(v);
				if (e != g.null_edge()) {
					REQUIRE(g.tail(e) == v);
					REQUIRE(distances(v) == distances(g.head(e)) + weight(e));
				}
			}
			for (auto e : g.edges())
				REQUIRE(!(distances(g.tail(e)) > distances(g.head(e)) + weight(e)));
		}
		WHEN("searching for the minimum spanning tree") {
			auto t = gt.random_vert(r);
			auto tree = g.minimum_tree_reaching_to(t, [](auto e) { return 1; });
			REQUIRE(tree.root() == t);
			// If the head of an edge is in the tree, then so must be the tail
			for (auto e : g.edges())
				REQUIRE((!tree.in_tree(g.head(e)) || tree.in_tree(g.tail(e))));
			// TODO: Use the cut lemma to verify this tree has minimal weight
		}
	}
}

SCENARIO("bi-adjacency lists behave properly", "[Bi_adjacency_list]") {
	using G = graph::Bi_adjacency_list;
	GIVEN("an empty graph") {
		G g;
		REQUIRE(g.order() == 0);
		REQUIRE(g.size() == 0);
		Bi_edge_graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("a self-edge is inserted and its head is erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_vert(v);
			REQUIRE(g.order() == 0);
			REQUIRE(g.size() == 0);
		}
		WHEN("an edge is inserted and its head is erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_vert(t);
			REQUIRE(g.order() == 1);
			REQUIRE(g.size() == 0);
		}
		WHEN("the graph is cleared") {
			gt.clear();
		}
	}
	GIVEN("a random graph") {
		std::mt19937 r;
		G g;
		const std::size_t M = 20, N = 100;
		for (std::size_t m = 0; m < M; ++m)
			g.insert_vert();
		for (std::size_t n = 0; n < N; ++n) {
			auto s = g.random_vert(r), t = g.random_vert(r);
			g.insert_edge(s, t);
		}
		REQUIRE(g.order() == M);
		REQUIRE(g.size() == N);
		In_edge_graph_tester gt{g};

		WHEN("a vertex is inserted and erased") {
			auto v = gt.insert_vert();
			gt.erase_vert(v);
		}
		WHEN("a self-edge is inserted and erased") {
			auto v = gt.insert_vert();
			auto e = gt.insert_edge(v, v);
			gt.erase_edge(e);
			gt.erase_vert(v);
		}
		WHEN("an edge is inserted and erased") {
			auto s = gt.insert_vert(),
				t = gt.insert_vert();
			auto e = gt.insert_edge(s, t);
			gt.erase_edge(e);
			gt.erase_vert(s);
			gt.erase_vert(t);
		}
		WHEN("all the edges are erased") {
			while (g.size())
				gt.erase_edge(gt.random_edge(r));
		}
		WHEN("the graph is cleared") {
			gt.clear();
		}
	}
}

#ifndef NDEBUG
SCENARIO("out-adjacency lists check preconditions when debugging", "[Out_adjacency_list]") {
	using G = graph::Out_adjacency_list;
	GIVEN("an empty graph") {
		G g;
		std::mt19937 r;
		WHEN("selecting a random vertex") {
			try {
				g.random_vert(r);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
		WHEN("selecting a random edge") {
			try {
				g.random_edge(r);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
	GIVEN("a graph") {
		G g;
		auto u = g.insert_vert(), v = g.insert_vert();
		g.insert_edge(u, v);
		WHEN("removing a reachable vertex") {
			try {
				g.erase_vert(v);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
	GIVEN("a graph with a negative edge weight") {
		G g;
		auto u = g.insert_vert(), v = g.insert_vert();
		g.insert_edge(u, v);
		auto weight = g.edge_map(-1.0);
		WHEN("searching for shortest paths from a vertex") {
			try {
				g.shortest_paths_from(u, weight);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
}
SCENARIO("in-adjacency lists check preconditions when debugging", "[In_adjacency_list]") {
	using G = graph::In_adjacency_list;
	GIVEN("an empty graph") {
		G g;
		std::mt19937 r;
		WHEN("selecting a random vertex") {
			try {
				g.random_vert(r);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
		WHEN("selecting a random edge") {
			try {
				g.random_edge(r);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
	GIVEN("a graph") {
		G g;
		auto u = g.insert_vert(), v = g.insert_vert();
		g.insert_edge(u, v);
		WHEN("removing a reachable vertex") {
			try {
				g.erase_vert(u);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
	GIVEN("a graph with a negative edge weight") {
		G g;
		auto u = g.insert_vert(), v = g.insert_vert();
		g.insert_edge(u, v);
		auto weight = g.edge_map(-1.0);
		WHEN("searching for shortest paths to a vertex") {
			try {
				g.shortest_paths_to(v, weight);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
}
SCENARIO("bi-adjacency lists check preconditions when debugging", "[Bi_adjacency_list]") {
	using G = graph::Bi_adjacency_list;
	GIVEN("an empty graph") {
		G g;
		std::mt19937 r;
		WHEN("selecting a random vertex") {
			try {
				g.random_vert(r);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
		WHEN("selecting a random edge") {
			try {
				g.random_edge(r);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
	GIVEN("a graph with a negative edge weight") {
		G g;
		auto s = g.insert_vert(), t = g.insert_vert();
		g.insert_edge(s, t);
		auto weight = g.edge_map(-1.0);
		WHEN("searching for shortest paths from a vertex") {
			try {
				g.shortest_paths_from(s, weight);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
		WHEN("searching for shortest paths to a vertex") {
			try {
				g.shortest_paths_to(t, weight);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
		WHEN("searching for a shortest path") {
			try {
				g.shortest_path(s, t, weight);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
		WHEN("searching for a shortest path in parallel") {
			try {
				g.parallel_shortest_path(s, t, weight);
				REQUIRE(false);
			} catch (graph::precondition_unmet) {}
		}
	}
}
#endif

#ifdef GRAPH_BENCHMARK
TEST_CASE("adjacency list", "[benchmark]") {
	using G = graph::Out_adjacency_list;
	static const std::size_t order = 1000;
	static const std::size_t size = 10000;
	std::mt19937 r;
	BENCHMARK("insert vertices") {
		G g;
		for (std::size_t i = 0; i < order; ++i)
			g.insert_vert();
		REQUIRE(g.order() == order);
	}
	BENCHMARK("insert self-edges") {
		G g;
		auto v = g.insert_vert();
		for (std::size_t i = 0; i < size; ++i)
			g.insert_edge(v, v);
		REQUIRE(g.size() == size);
	}
	BENCHMARK("insert random edges") {
		G g;
		for (std::size_t i = 0; i < order; ++i)
			g.insert_vert();
		for (std::size_t i = 0; i < size; ++i) {
			auto u = g.random_vert(r), v = g.random_vert(r);
			g.insert_edge(u, v);
		}
		REQUIRE(g.order() == order);
		REQUIRE(g.size() == size);
	}

	G g;
	for (std::size_t i = 0; i < order; ++i)
		g.insert_vert();
	for (std::size_t i = 0; i < size; ++i) {
		auto u = g.random_vert(r), v = g.random_vert(r);
		g.insert_edge(u, v);
	}

	BENCHMARK("query adjacencies") {
		std::size_t total_degrees = 0;
		for (auto v : g.verts())
			for (auto e : g.out_edges(v))
				++total_degrees;
		REQUIRE(total_degrees == size);
	}
	BENCHMARK("find single-source shortest paths") {
		auto weight = g.edge_map(0.0);
		for (auto e : g.edges())
			weight[e] = std::uniform_real_distribution<double>{}(r);
		for (auto s : g.verts()) {
			auto [_, distance] = g.shortest_paths_from(s, weight);
			REQUIRE(distance(s) == 0);
		}
	}
	BENCHMARK("erase edges") {
		for (std::size_t i = 0; i < size; ++i)
			g.erase_edge(g.random_edge(r));
		REQUIRE(g.size() == 0);
	}
	BENCHMARK("erase vertices") {
		for (std::size_t i = 0; i < order; ++i)
			g.erase_vert(g.random_vert(r));
		REQUIRE(g.order() == 0);
	}
}
#endif
