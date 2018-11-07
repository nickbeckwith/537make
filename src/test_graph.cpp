#include "catch.hpp"

// include c headers
extern "C" {
	#include "graph.h"
	#include "tools.h"
	#include "build_spec.h"
	#include <string.h>
}

TEST_CASE("Cyclic graph identification test 1") {
	graph_t *graph = initGraph();
	vertex_t *vertex1 = initVertex(nullptr);
	vertex_t *vertex2 = initVertex(nullptr);
	addVertex(graph, vertex1);
	addVertex(graph, vertex2);
	addEdge(vertex1, vertex2);
	addEdge(vertex2, vertex1);
	REQUIRE(isCyclic(graph) != nullptr);
}

TEST_CASE("Cyclic graph identification test 2") {
	graph_t *graph = initGraph();
	vertex_t *vertex1 = initVertex(nullptr);
	vertex_t *vertex2 = initVertex(nullptr);
	vertex_t *vertex3 = initVertex(nullptr);
	vertex_t *vertex4 = initVertex(nullptr);
	addVertex(graph, vertex1);
	addVertex(graph, vertex2);
	addVertex(graph, vertex3);
	addVertex(graph, vertex4);
	addEdge(vertex1, vertex2);
	addEdge(vertex2, vertex3);
	addEdge(vertex3, vertex4);
	addEdge(vertex4, vertex1);
	REQUIRE(isCyclic(graph) != nullptr);
}

TEST_CASE("acyclic graph identification") {
	graph_t *graph = initGraph();
	vertex_t *vertex1 = initVertex(nullptr);
	vertex_t *vertex2 = initVertex(nullptr);
	vertex_t *vertex3 = initVertex(nullptr);
	vertex_t *vertex4 = initVertex(nullptr);
	addVertex(graph, vertex1);
	addVertex(graph, vertex2);
	addVertex(graph, vertex3);
	addVertex(graph, vertex4);
	addEdge(vertex1, vertex2);
	addEdge(vertex2, vertex3);
	addEdge(vertex3, vertex4);
	REQUIRE(isCyclic(graph) == nullptr);
}

TEST_CASE("find vertex usage") {
	graph_t *graph = initGraph();
	build_t *build1 = initBuild();
	addTarget(build1, "test this");
	build_t *build2 = initBuild();
	addTarget(build2, "test this");
	vertex_t *vertex1 = initVertex(nullptr);
	vertex_t *vertex2 = initVertex(nullptr);
	vertex_t *vertex3 = initVertex(nullptr);
	vertex_t *vertex4 = initVertex(build1);
	addVertex(graph, vertex1);
	addVertex(graph, vertex2);
	addVertex(graph, vertex3);
	addVertex(graph, vertex4);
	addEdge(vertex1, vertex2);
	addEdge(vertex2, vertex3);
	addEdge(vertex3, vertex4);
	REQUIRE(strcmp(findVertex(graph, build2)->build->target, "test this") == 0);
}

TEST_CASE("Don't find vertex usage") {
	graph_t *graph = initGraph();
	build_t *build1 = initBuild();
	addTarget(build1, "test not");
	build_t *build2 = initBuild();
	addTarget(build2, "test this");
	vertex_t *vertex1 = initVertex(nullptr);
	vertex_t *vertex2 = initVertex(nullptr);
	vertex_t *vertex3 = initVertex(nullptr);
	vertex_t *vertex4 = initVertex(build1);
	addVertex(graph, vertex1);
	addVertex(graph, vertex2);
	addVertex(graph, vertex3);
	addVertex(graph, vertex4);
	addEdge(vertex1, vertex2);
	addEdge(vertex2, vertex3);
	addEdge(vertex3, vertex4);
	REQUIRE(findVertex(graph, build2) == nullptr);
	free(graph);
}