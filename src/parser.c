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
int * getTargetIndex(const char *targetLine);
char * getCmd(char *cptr);
char * fgetsWrapper(char *str, int n, FILE *stream);
void printParserErrorAndExit(const char *line);
build_t * readBuild(FILE *file_pointer);


list_t * readAll(const char *filename) {
	// open file and handle errors
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		fprintf(stderr, "Error: Makefile could not be opened\n");
		exit(EXIT_FAILURE);
	}

	// create build list and populate it
	list_t *list = initList();
	build_t *build;
	while ((build = readBuild(fptr)) != NULL) {
		addElem(list, build);
	}
	if (list->len == 0) {
		fprintf(stderr, "make: *** No Targets.\tStop");
		exit(EXIT_FAILURE);
	}
	return list;
}

/**
 * Reads a single build
 * @param file_pointer
 * @return NULL if reached EOF before target received
 */
build_t * readBuild(FILE *file_pointer) {
	build_t *build = initBuild();


    // get line of targets and dependents
	char * line_ptr;
	line_ptr = (char *) mallocWrapper(sizeof(char) * MAX_BUF_LEN);
    if (fgetsWrapper(line_ptr, MAX_BUF_LEN, file_pointer) == NULL) {
        return NULL;        // if EOF
    }

	// Get target index and split the strings at that index to get target and dependents
	int *index_arr = getTargetIndex(line_ptr);
    char *target = (char *) mallocWrapper(sizeof(char) * MAX_BUF_LEN);
    strcpy(target, line_ptr);
    target[index_arr[0]] = '\0';
	addTarget(build, target);
	// increment line_ptr to get dependents
	line_ptr = line_ptr + index_arr[1] + 1;

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
			printParserErrorAndExit(line_ptr);
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
 * @param assumes we're the line we're on contains the target
 * @return the first index is after target and the second index is at the colon
 */
int * getTargetIndex(const char *targetLine) {
	// first ensure first char is a readable char
	int *ret_arr = (int *) mallocWrapper(2 * sizeof(int));
	for (int i = 0; targetLine[i] != '\0'; i++) {
		if (isalpha(targetLine[i]) | isdigit(targetLine[i])) {
			// it's okay
		} else if (targetLine[i] == ':') {
			ret_arr[0] = i;
			ret_arr[1] = i;
			return ret_arr;
		} else {
			// must be able to handle "all   : main.o" (multiple spaces"
			for (int j = i; targetLine[j] != '\0'; j++) {
				if (isalpha(targetLine[j]) | isdigit(targetLine[j])) {
					printParserErrorAndExit(targetLine);
				} else if (targetLine[j] == ':') {
					ret_arr[0] = i;
					ret_arr[1] = j;
					return ret_arr;
				}
			}
			printParserErrorAndExit(targetLine);
		}
	}
	printParserErrorAndExit(targetLine);
	return NULL;      // unreachable but clang doesn't realize it
}

/**
 * Fgets wrapper increments line counter
 * returns null if EOF
 * Increments until there's a non null line
 * Only place where we deal with lines possibly > 1024
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
			// make sure to print out entire line
			while (!strchr(str, '\n')) {
				fprintf(stderr, "%d: Invalid line: %s", line_number, str);
				fgets(str, n, stream);
			}
			fprintf(stderr, "%d: Invalid line: %s", line_number, str);
			exit(EXIT_FAILURE);
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
 * Only should be used after getting line with fgetsWrapper
 * @param line error to be printed
 */
void printParserErrorAndExit(const char *line) {
	fprintf(stderr, "%d: Invalid line: %s", line_number, line);
	exit(EXIT_FAILURE);
}

