#ifndef P3_GRAPH_H
#define P3_GRAPH_H

#include "build_spec.h"
#include "tools.h"
#include "list.h"

typedef struct vertex_t {
	build_t *build;
	int visited;
	list_t *edges;
} vertex_t;

typedef struct graph_t {
	list_t *vertices;
	int num_vertices;
}graph_t;


/**
 *
 * @return an empty initialized graph
 * with num vertices set to zero
 */
graph_t *initGraph();

/**
 * frees what was allocated in graph
 * @param graph
 */
void freeGraph(graph_t *graph);

/**
 * Allocates a vertex with build. Sets visited to zero
 * @param build
 */
vertex_t * initVertex(build_t *build);

/**
 * frees what was dynamically allocated in initVertex
 * @param node
 */
void freeVertex(vertex_t *vertex);

/**
 * Adds vertex to graph_t.vertices
 * Assumes there are no duplicate targets
 * @param graph graph that vertex is being added to
 * @param vertex vertex being added to graph
 */
void addVertex(graph_t *graph, vertex_t *vertex);

/**
 * Creates an edge from "from" and to "to"
 * @param from
 * @param to
 */
void addEdge(vertex_t *from, vertex_t *to);

/**
 * @param build whose target is going to be compared
 * @returns vertex that matches build. NULL if no match
 */
vertex_t *findVertex(graph_t *graph, build_t *build);

/**
 * @param target whose target is going to be compared
 * @returns vertex that matches target. NULL if no match
 */
vertex_t *findVertexByTarget(graph_t *graph, const char * target);

/**
 * resets all vertices of graph to not visited
 * @param graph
 */
void clearVisited(graph_t *graph);

/**
 * Checks if graph is cyclic. Clears visited
 * @param graph
 * @return the first twice visited node. If not cyclic then return NULL
 */
vertex_t * isCyclic(graph_t *graph);

/**
 * returns 1 if the two vertices are equal 0 otherwise
 * The equality class is if the targets within the builds are equal
 * Null characters are not equal
 * @param vertex1
 * @param vertex2
 * @return
 */
int isVertexEqual(vertex_t *vertex1, vertex_t *vertex2);

/**
 * returns 1 if the two vertices are equal 0 otherwise
 * The equality class is if the targets within the builds are equal
 * Null characters are not equal
 * @param vertex
 * @param target
 * @return
 */
int isVertexEqualByTarget(vertex_t *vertex, const char *target);


#endif //P3_GRAPH_H
