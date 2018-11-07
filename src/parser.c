#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "build_spec.h"
#include "tools.h"
// Privately defined
#define MAX_FILE_LEN 255
#define MAX_CMD_LEN  1024     // [might change to SC_ARG_MAX]
#define MAX_OTHERS_LEN 1024

// Global variables
int line_number = 0;

// Privately defined
char * getTarget(FILE *fptr);
char * getDep(char *cptr);
char * getCmd(char *cptr);
void checkforTab(char *cptr);

build_t * readBuild(FILE *file_pointer) {
		build_t *return_file = buildInit();
        if (file_pointer == NULL) {
                printf("Error: Makefile could not be opened\n");
                exit(1);
        }

        // Target: Reads first word, checks if valid target, stores valid target
        char * temp_target_ptr;
        temp_target_ptr = getTarget(file_pointer);
        return_file->target = getTarget(file_pointer);;      


        // Dependents: Creates line pointer, set pointers in return file to each word
        char * line_ptr;
        line_ptr = (char *) mallocWrapper(sizeof(char) * MAX_FILE_LEN);
        fgets(line_ptr, MAX_FILE_LEN, file_pointer);
		line_number++;
        char * temp_dependent_ptr;
		// 
        while (*line_ptr != '\0') {
                temp_dependent_ptr = getDep(line_ptr);
                addDependent(getDep(file_pointer));
                while (*(temp_dependent_ptr) != '\0') {
                        temp_dependent_ptr++;
                        line_ptr++;
                }
                line_ptr++;
         }
        
         // Cmds: Set line pointer to new line, check for tab, set pointers in return file to each word
        fgets(line_ptr, MAX_FILE_LEN, file_pointer);
		line_number++;
        checkforTab(line_ptr);
		// line_
        while (*line_ptr != '\0') {
                temp_dependent_ptr = getCmd(line_ptr);
                addCmd(getCmd(file_pointer));
                while (*(temp_dependent_ptr) != '\0') {
                        temp_dependent_ptr++;
                        line_ptr++;
                }
                line_ptr++;
         }

        fclose(file_pointer);
        return return_file;
}

target1: dep1 dep2
	cmd1 cmd2

dep1: dep3 dep4


/**
 * Assumes this is called while fptr is on first column
 * Advances fptr to target and consumes "target:"
 * and returns "target:"
 */
char * getTarget(FILE *fptr) {
        char *target = (char *) mallocWrapper(MAX_FILE_LEN * sizeof(char));
		// TODO read line with fgets
		// TODO consume lines until fscanf returns 1
		// TODO if we don't reach ret_val of 1 then check EOF
        fscanf(fptr, "%s", target);

        char * valid_target_checker = target;
        while(*(valid_target_checker + 1) != NULL) {
                valid_target_checker = valid_target_checker + 1;
        }

        if(*valid_target_checker != ':') {
                printf("makefile:1: *** missing separator.  Stop.\n");
                exit(1);
        }

        return target;
}

char * getDep(char *cptr) {
        char *cmd = (char *) mallocWrapper(MAX_OTHERS_LEN * sizeof(char));
        sscanf(cptr, "%s", cmd);
        return cmd;
}

char * getCmd(char *cptr) {
        char *cmd = (char *) mallocWrapper(MAX_CMD_LEN * sizeof(char));
        sscanf(cptr, "%s", cmd);
        return cmd;
}

void checkforTab(char *cptr) {
        int space_counter = 0;
        if (*cptr != '\t') {
                while(*(cptr + space_counter) == ' ') {
                        space_counter++;
                }
                printf("makefile:2: *** missing separator (did you mean TAB instead of %d spaces?).  Stop.\n", space_counter);
                exit(1);
        }
}

/**
 * Fgets wrapper increments line counter
 * returns null if EOF
 * Increments until there's a non null line
 **/
char * fgetsWrapper(char *str, int n, FILE *stream) {
	char *line = (char *) mallocWrapper(n * sizeof(char));
	do {
		line = fgets(str, n, stream);
		line_number++;
	} while (line != NULL | line[0] == '\n');

	// if line was null then do error checking or find eof
	if (line == NULL) {
		if (feof(stream)) {
			return NULL;
		} else {
			perror("Error: ");
			exit(EXIT_FAILURE);
		}
	}
	return line;
}	
// TODO Write fscanf wrapper that takes fscanf output as input and
// does error handling. i.e. scanErr(fscanf(...));
