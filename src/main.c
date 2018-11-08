#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "main.h"
#include "list.h"
#include "parser.h"
#include "graph.h"
#include "command.h"

/**
 * Returns file name needed to be used
 * If non available, quit program and print error
 * @param customName custom name NULL if not used
 * @return
 */
const char *getMakeFileName(const char *customName);

/**
 * Returns file name needed to be used
 * If non available, quit program and print error
 * @param customName custom name NULL if not used
 * @return
 */
const char *getMakeFileName(const char *customName);

/**
 *
 * @param f1
 * @param f2
 * @return returns 0 > if f1 is newer than f2
 * returns 0 < if f2 is newer than 1
 * returns 0 if f1 is the same age as f2
 */
double getNewer(const char *f1, const char *f2) ;

/**
 * tries to run commands in vertex using make dependency rules
 * @param vertex
 * @return 0> if command is ran
 */
int tryRunVertex(graph_t *graph, vertex_t *vertex) ;

int isFile(const char *filename) {
	return access(filename, F_OK) == 0;
}

int main(int argc, char *argv[]) {
	// options processing
	int opt;
	char *customName = NULL;
	char *target;
	static char usage[] = "usage: make [target] [-f name]\n";
	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
			case 'f':
				customName = optarg;
				break;
			default:
				fprintf(stderr, "%s", usage);
				exit(EXIT_FAILURE);
		}
	}
	if ((optind != argc - 1) && argc != 1) {
		fprintf(stderr, "%s", usage);
		exit(EXIT_FAILURE);
	} else {
		target = argv[optind];
	}
	// end options processing

	// start text file processing create list of build targets
	const char *fileName = getMakeFileName(customName);
	list_t *list = readAll(fileName);

	// check list if there are repeating targets
	node_t *ptr1;
	node_t *ptr2;
	ptr1 = list->head;
	// pick a node to compare with the rest
	while (ptr1 != NULL && ptr1->next != NULL) {
		ptr2 = ptr1;
		// iterate through the rest to find a comparison
		while ((ptr2 = ptr2->next) != NULL) {
			if (isBuildEqual((build_t *) ptr1->data, (build_t *) ptr2->data)) { // if equality is found
				fprintf(stderr, "make: *** Duplicate target: '%s'\tStop", ((build_t*)ptr1->data)->target);
				exit(EXIT_FAILURE);

			} // else if this ptr2 isn't equal
		} // else move to next ptr1 to compare
		ptr1 = ptr1->next;
	}

	// create graph and populate with targets detect phony targets
	graph_t *graph = initGraph();
	ptr1 = list->head;
	while (ptr1 != NULL) {
		addVertex(graph, initVertex(ptr1->data));       // each target as a vertex
		// check if each target is phony
		if (isFile(((build_t *) ptr1->data)->target)) {
			((build_t *) ptr1)->isPhony = 0;
		} else {
			((build_t *) ptr1)->isPhony = 1;
		}
		ptr1 = ptr1->next;
	}

	// populate with dependencies create builds for non-phony dependencies and add to vertices
	ptr1 = graph->vertices->head;
	build_t *temp_build;
	vertex_t *temp_vertex;
	while (ptr1 != NULL) {
		// for each dependency in list, hook edge up
		ptr2 = ((vertex_t *) ptr1->data)->build->dependents->head;
		while (ptr2 != NULL) {
			// hook edge up if target exists
			if ((temp_vertex = findVertexByTarget(graph, ptr2->data)) != NULL) {
				addEdge(ptr1->data, temp_vertex);
			}
			ptr2 = ptr2->next;
		} // after adding all dependencies, increment to next vertex
		ptr1 = ptr1->next;
	}

	// next check if graph is cyclic and exit if so
	vertex_t *err_vertex;       // if this isn't null, we got a cycle
	if ((err_vertex = isCyclic(graph)) != NULL) {
		fprintf(stderr, "make: *** Cycle found at target: '%s'\tStop", err_vertex->build->target);
		exit(EXIT_FAILURE);
	}

	// finally run tryRunVertex on desired target if default, run on first vertex
	if (target != NULL) {
		tryRunVertex(graph, findVertexByTarget(graph, target));
	} else {
		tryRunVertex(graph, graph->vertices->head->data);
	}
}

/**
 * tries to run commands in vertex using make dependency rules
 * @param vertex
 * @return 0> if command is ran
 */
int tryRunVertex(graph_t *graph, vertex_t *vertex) {
	int run_cmd = 0;
	build_t *build;
	// first traverse all vertices
	node_t *ptr = vertex->edges->head;
	while (ptr != NULL) {
		run_cmd += tryRunVertex(graph, ptr->data);
		ptr = ptr->next;
	}
	build = vertex->build;
	ptr = build->dependents->head;
	vertex_t *temp_vertex;
	if (isPhony(build) == 0) {
		while (ptr != NULL) {   // traverse across dependents of each build
			if (isFile(ptr->data)) {    // if dependency is a file
				// check which file is most recent. if target is not associated with file yet, run_cmd
				if (getNewer(build->target, ptr->data) < 0) {    // dependency is newer than target
					run_cmd++;
				}
			} else if ((temp_vertex = findVertexByTarget(graph, ptr->data)) != NULL) { // dep is a target
				if (temp_vertex->build->out_of_date == 1) {
					run_cmd++;
				}
			}
		}
	} else { // else target name phony and should update
		run_cmd++;
	}
	//  if we have a reason to update, update
	if (run_cmd > 0) {
		vertex->build->out_of_date = 1;
		runAllCommands(vertex->build);
	}
	return run_cmd;
}

// if target is a file, dependency must be newer than target to run


/**
 *
 * @param f1
 * @param f2
 * @return returns 0 > if f1 is newer than f2
 * returns 0 < if f2 is newer than 1
 * returns 0 if f1 is the same age as f2
 */
double getNewer(const char *f1, const char *f2) {
	struct stat sb1, sb2;
	if (stat(f1, &sb1) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}
	if (stat(f2, &sb2) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}
	double ret_val = 0;
	ret_val = difftime(sb1.st_mtim.tv_sec, sb2.st_mtim.tv_sec);
	if (ret_val != 0) {
		return ret_val;
	} else {
		return difftime(sb1.st_mtim.tv_nsec, sb2.st_mtim.tv_nsec);
	}
}

const char *getMakeFileName(const char *customName) {
if (customName == NULL) {
	if (isFile("makefile")) {
		return "makefile";
	} else if (isFile("Makefile")) {
		return "Makefile";
	} else {

	}
} else {
	if (isFile(customName)) {
		return customName;
	} // else error no makefile found
	fprintf(stderr, "make: *** No makefile found.\tStop");
	exit(EXIT_FAILURE);
}
}





