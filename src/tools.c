// Nick Beckwith, netID: nbeckwith2; Matthew Strimaitis, netID: strimaitis
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"

void * mallocWrapper(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

void doNothing(__attribute__((unused)) void *ptr) {
	// do nothing
}
