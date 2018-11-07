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
	vertex_t *vertices;
	int num_vertices;
}graph_t;


/**
 *
 * @param max_vertices max number of vertices allowed
 * @return an empty initialized graph with set number of vertices allowed
 * and num vertices set to zero, and adjacency list of vertices dynamically allocated
 */
graph_t *initGraph(int max_vertices);

/**
 * frees what was allocated in graph
 * @param graph
 */
void freeGraph(graph_t graph);

/**
 * Allocates a vertex with build. Sets visited to zero
 * @param build
 */
vertex_t * initVertex(build_t *build);

/**
 * frees what was dynamically allocated in initVertex
 * @param node
 */
void freeVertex(vertex_t *node);

/**
 * Adds vertex to graph_t.vertices
 * @param vertex vertex being added to graph
 * @return -1 vertex already existed. Does not write
 */
int addVertex(vertex_t *vertex, vertex_t *child);



/**
 * Checks if graph is cyclic. Assumes all visited is zero. Returns all visited to zero after.
 * @param graph
 * @return the first twice visited node. If not cyclic then return NULL
 */
vertex_t * isCyclic(graph_t graph);


#endif //P3_GRAPH_H
