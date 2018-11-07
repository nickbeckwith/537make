#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "list.h"
#include "parser.h"

int main(int argc, char *argv[]) {
	// options processing
	int opt;
	char *customName;
	char *target;
	static char usage[] = "usage: make [target] [-f name]\n";
	while ((opt == getopt(argc, argv, "f:")) != -1) {
		switch (opt) {
			case 'f':
				customName = optarg;
				break;
			case '?':
				if (optopt == 'f') {
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
					fprintf(stderr, usage);
					exit(EXIT_FAILURE);
				}
		}
	}
	if (argc != 1) {
		fprintf(stderr, usage);
		exit(EXIT_FAILURE);
	} else {
		target = argv[0];
	}



}

/**
 * returns 1 if filename exists and 0 if not
 * @param filename
 * @return
 */
int isFile(const char *filename) {
	return access(filename, F_OK) == 0;
}

/**
 * Returns file name needed to be used
 * If non available, quit program and print error
 * @param customName custom name NULL if not used
 * @return
 */
char *getMakeFileName(const char *customName) {
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



