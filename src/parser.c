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


/**
 * checks if char is valid in filename
 * @param c
 * @return
 */
int isValidChar(char c) ;

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
		addElem(list, build, &doNothing);        // builds will be duplicated to graph and freed there
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
	char *line_ptr_val = line_ptr;        // copy of pre-strtok (immutable)
    if (fgetsWrapper(line_ptr, MAX_BUF_LEN, file_pointer) == NULL) {
    	free(line_ptr);
    	freeBuild(build);
        return NULL;        // if EOF
    }

	// Get target index and split the strings at that index to get target and dependents
	int *index_arr = getTargetIndex(line_ptr);
    line_ptr[index_arr[0]] = '\0';
    char *target = strdup(line_ptr);
	addTarget(build, target);
	// increment line_ptr to get dependents
	line_ptr = line_ptr + index_arr[1] + 1;
	free(index_arr);

	// Get dependents tokenized
	char **tokens = (char **) mallocWrapper(MAX_BUF_LEN * sizeof(char *));
	int tokens_count = 0;
	char *token;
	token = strtok(line_ptr, " \n");
	if (token == NULL) {
		tokens[tokens_count] = NULL;
	} else {
		tokens[tokens_count] = strdup(token);     // create memory for a token
	}
	while (tokens[tokens_count] != NULL) {
		tokens_count++;
		token = strtok(NULL, " \n");
		if (token == NULL) {
			tokens[tokens_count] = NULL;
		} else {
			tokens[tokens_count] = strdup(token);       // don't want to malloc a null pointer
		}
	}
	// line isn't needed anymore, we malloced each dependency and target
	free(line_ptr_val);

	// add all dependents 1 by 1
	for (int i = 0; i < tokens_count; i++) {
		addDependent(build, tokens[i]);
	}
	free(tokens);

	long file_ptr_locaiton;
	// Cmds: Set line pointer to new line, check for tab, set pointers in return file to each word
	while (1) {
		line_ptr = (char *) mallocWrapper(sizeof(char) * MAX_BUF_LEN);
		file_ptr_locaiton = ftell(file_pointer);
		if (fgetsWrapper(line_ptr, MAX_BUF_LEN, file_pointer) == NULL) {
			free(line_ptr);
			return build;        // if EOF
		}
		// if alpha/digit character, build is done. Make sure to reset
		// file pointer to where it was before consuming target line
		if (isValidChar(line_ptr[0])) {
			fseek(file_pointer, file_ptr_locaiton, SEEK_SET);       // return to place before target
			free(line_ptr);
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
		addCmd(build, strdup(&line_ptr[1]));
		free(line_ptr);
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
		if (isValidChar(targetLine[i])) {       // maybe next time go through
			// it's okay                                                                    // all fat32 possible filenames
		} else if (targetLine[i] == ':') {
			ret_arr[0] = i;
			ret_arr[1] = i;
			return ret_arr;
		} else {
			// must be able to handle "all   : main.o" (multiple spaces"
			for (int j = i; targetLine[j] != '\0'; j++) {
				if (isValidChar(targetLine[j])) {
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
		if (str[0] == '#') {    // if comment
			str[0] = '\n';
		}
		// if line doesn't have a new line and isn't EOF then
		if (!strchr(str, '\n') && !feof(stream)) {
			while (!strchr(str, '\n')) {
				fprintf(stderr, "%d: Invalid line: %s", line_number, str);
				fgets(str, n, stream);
			}
			fprintf(stderr, "%d: Invalid line: %s", line_number, str);
			exit(EXIT_FAILURE);
		}
	} while ((str[0] == '\n') && !feof(stream));
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

/**
 * checks if char is valid in filename
 * @param c
 * @return
 */
int isValidChar(char c) {
	return isalpha(c) || isdigit(c) || c == '.' || c == '_';
}
