#include <stdio.h>
#include "parser.h"
#include "build_spec.h"
#include "tools.h"

build_list_t * readAll(char *filename) {
	build_list_t * return_file;
	FILE * fptr;
}

/**
 * Assumes this is called while fptr is on first column
 * Advances fptr to target and consumes "target:"
 * and returns "target"
 */
char * getTarget(FILE *fptr) {
	char *target = (char *) mallocWrapper(MAX_FILE_SIZE * sizeof(char));
	fscanf(fptr, "%s:", target);
	return target;
	//TODO ensure that fscanf only gets target if target is on first column
}

/**
 * Get dependents. Assumes fptr is currently on char after ':'
 * Returns a single cmd and advances past single cmd returns null
 * if there are no more commands left
 */
char * getCmd(FILE *fptr) {
	char *cmd = (char *) mallocWrapper(MAX_SIZE_OTHER * sizeof(char));
	fscanf(fptr, "%s", cmd);
	return cmd;
}


// TODO Write fscanf wrapper that takes fscanf output as input and 
// does error handling. i.e. scanErr(fscanf(...));
