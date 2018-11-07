#include <stdio.h>
#include "graph.h"
#include "tools.h"


graph_t *initGraph() {
	graph_t *graph = (graph_t *) mallocWrapper(sizeof(graph_t));
	graph->num_vertices = 0;
	graph->vertices = initList();
	return graph;
}

void freeGraph(graph_t *graph) {
	freeList(graph->vertices);
	free(graph);
}

vertex_t *initVertex(build_t *build) {
	vertex_t *vertex = (vertex_t *) mallocWrapper(sizeof(vertex));
	vertex->build = build;
	vertex->visited = 0;
	vertex->edges = initList();
	return vertex;
}

void freeVertex(vertex_t *vertex) {
	freeList(vertex->edges);
	free(vertex);
}

void addVertex(graph_t *graph, vertex_t *vertex) {
	addElem(graph->vertices, vertex);
}

void addEdge(vertex_t *from, vertex_t *to) {
	addElem(from->edges, to);
}

vertex_t *findVertex(graph_t *graph, build_t *build) {
	vertex_t *vertex = initVertex(build);
	node_t *node_ptr = graph->vertices->head;
	while (node_ptr->next != NULL) {
		// if targets of two builds are equal
		if (isVertexEqual(vertex, node_ptr->data)) {
			return node_ptr->data;
		} // else
		node_ptr = node_ptr->next;
	}
	return NULL;
}

void clearVisited(graph_t *graph) {
	node_t *node_ptr = graph->vertices->head;
	if (node_ptr != NULL) {
		do {
			((vertex_t *) node_ptr->data)->visited = 0;
		} while (node_ptr->next != NULL);
	}
}

/**
 * Assumes visited is reset
 * @param vertex vertex to start at
 * @return value > 0 if a cycle is found
 */
int isCyclicHelper(vertex_t *vertex) {
	vertex->visited++;
	if (vertex->edges->len == 0) {
		return 0;
	} else if (vertex->visited > 1) {
		// found a cycle
		return 1;
	} else {
		// check all edges and keep track of returns
		node_t *node_ptr = vertex->edges->head;
		int count = 0;
		while (node_ptr != NULL) {
			count += isCyclicHelper((vertex_t *) node_ptr->data);
			node_ptr = node_ptr->next;
		}
		return count;
	}
}

vertex_t * isCyclic(graph_t *graph) {
	// go through each vertex of graph skipping ones that have been visited
	node_t *adj_node_ptr1 = graph->vertices->head;
	while (adj_node_ptr1 != NULL) {
		clearVisited(graph);
		// if we find a graph that is cyclic
		if (isCyclicHelper(adj_node_ptr1->data) > 0) {
			// find the location of that vertex
			node_t *adj_node_ptr2 = graph->vertices->head;
			while (adj_node_ptr2 != NULL) {
				if (((vertex_t *)adj_node_ptr2->data)->visited > 1) {
					return adj_node_ptr2->data;
				}
				adj_node_ptr2 = adj_node_ptr2->next;
			}
			fprintf(stderr, "Thought graph was cyclic but can't find where");
			exit(EXIT_FAILURE);
		} // else acyclic
		// skip all already visited vertices
		do {
			adj_node_ptr1 = adj_node_ptr1->next;
		} while ((((vertex_t *)adj_node_ptr1->data)->visited == 1) & adj_node_ptr1 != NULL);
		// while we pass visited nodes (we know the outcome) and while our ptr isn't null
	}
	return NULL;
}

int isVertexEqual(vertex_t *vertex1, vertex_t *vertex2) {
	return isBuildEqual(vertex1->build, vertex2->build);
}








