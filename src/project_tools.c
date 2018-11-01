#include <stdlib.h>
#include <stdio.h>
#include "project_tools.h"

inline void * mallocWrapper(size_t size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	return ptr;
}