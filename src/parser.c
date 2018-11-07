#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "build_spec.h"
#include "tools.h"
// Privately defined


// this converts to string
#define STR_(X) #X

// this makes sure the argument is expanded before converting to string
#define STR(X) STR_(X)

// Global variables
int line_number;

// Privately defined
int getTargetIndex(const char *targetLine);
char * getCmd(char *cptr);
char * fgetsWrapper(char *str, int n, FILE *stream);
void printParserError(char *err);
build_t * readBuild(FILE *file_pointer);


build_list_t * readAll(char *filename) {
	// open file and handle errors
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error: Makefile could not be opened\n");
		exit(EXIT_FAILURE);
	}

	// create build list and populate it
	build_list_t *list = buildListInit();
	build_t *build;
	while ((build = readBuild(fptr)) != NULL) {
		addBuild(list, build);
	}
	if (list->len == 0) {
		return NULL;
	}
	return list;
}

/**
 * Reads a single build
 * @param file_pointer
 * @return NULL if reached EOF before target received
 */
build_t * readBuild(FILE *file_pointer) {
	build_t *build = buildInit();


    // get line of targets and dependents
	char * line_ptr;
	line_ptr = (char *) mallocWrapper(sizeof(char) * MAX_BUF_LEN);
    if (fgetsWrapper(line_ptr, MAX_BUF_LEN, file_pointer) == NULL) {
        return NULL;        // if EOF
    }

	// Get target index and split the strings at that index to get target and dependents
	int targetIndex = getTargetIndex(line_ptr);
    char *target = (char *) mallocWrapper(sizeof(char) * MAX_BUF_LEN);
    strcpy(target, line_ptr);
    target[targetIndex] = '\0';
	addTarget(build, target);
	// increment line_ptr to get dependents
	line_ptr = line_ptr + targetIndex + 1;

	// Get dependents tokenized
	char **tokens = (char **) mallocWrapper(MAX_BUF_LEN * sizeof(char *));
	int tokens_count = 0;
	tokens[tokens_count] = strtok(line_ptr, " \n");     // initialize and pass the first token
	while (tokens[tokens_count] != NULL) {
		tokens_count++;
		tokens[tokens_count] = strtok(NULL, " \n");
	}

	// add all dependents 1 by 1
	for (int i = 0; i < tokens_count; i++) {
		addDependent(build, tokens[i]);
	}

	long file_ptr_locaiton;
	// Cmds: Set line pointer to new line, check for tab, set pointers in return file to each word
	while (1) {
		line_ptr = (char *) mallocWrapper(sizeof(char) * MAX_BUF_LEN);
		file_ptr_locaiton = ftell(file_pointer);
		if (fgetsWrapper(line_ptr, MAX_BUF_LEN, file_pointer) == NULL) {
			return build;        // if EOF
		}
		// if alpha/digit character, build is done. Make sure to reset
		// file pointer to where it was before consuming target line
		if (isalpha(line_ptr[0]) | isdigit(line_ptr[0])) {
			fseek(file_pointer, file_ptr_locaiton, SEEK_SET);       // return to place before target
			return build;
		}
		// check for tab
		if (line_ptr[0] != '\t') {
			printParserError("No tab found when expecting command");
		}
		// replace new line at end of command
		for (int i = 0; line_ptr[i] != '\0'; i++) {
			if (line_ptr[i] == '\n') {
				line_ptr[i] = '\0';
			}
		}
		addCmd(build, &line_ptr[1]);
	}
}

/**
 * Modifies
 * @param targetLine is modified to remove "target:"
 * @return index where ':' is located
 */
int getTargetIndex(const char *targetLine) {
	// first ensure first char is a readable char
	for (int i = 0; targetLine[i] != '\0'; i++) {
		if (isalpha(targetLine[i]) | isdigit(targetLine[i])) {
			// it's okay
		} else if (targetLine[i] == ':') {
			return i;
		} else {
			printParserError("Target not found");
		}
	}
	printParserError("Target not found");
	return -1;      // unreachable but clang doesn't realize it
}

/**
 * Fgets wrapper increments line counter
 * returns null if EOF
 * Increments until there's a non null line
 **/
char * fgetsWrapper(char *str, int n, FILE *stream) {
	char *status;
	do {
		status = fgets(str, n, stream);
		line_number++;
		if (status == NULL) {
			break;          // if error or EOF get out of loop
		}
		// if line doesn't have a new line and isn't EOF then
		if (!strchr(str, '\n') & !feof(stream)) {
			char *status_tmp;
			printParserError("line too large");
		}
	} while (str[0] == '\n' & !feof(stream));
	if (status == NULL) {
		line_number--;          // don't count last line
		if (feof(stream)) {
			return NULL;
		} else {
			perror("Error: ");
			exit(EXIT_FAILURE);
		}
	}
	return str;
}

/**
 * Prints error message with line number and exits with failure
 * @param err error to be printed
 * @param line_number
 */
void printParserError(char *err) {
	fprintf(stderr, "%d: Invalid line: %s\n", line_number, err);
	exit(EXIT_FAILURE);
}

