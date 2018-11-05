#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "build_spec.h"
#include "tools.h"
// Privately defined
#define MAX_FILE_LEN 255
#define MAX_CMD_LEN  1024     // [might change to SC_ARG_MAX]
#define MAX_OTHERS_LEN 1024

// Privately defined
char * getTarget(FILE *fptr);
char * getDep(char *cptr);
char * getCmd(FILE *fptr);

build_t * readAll(char *filename) {
        build_t * return_file = NULL;
        buildInit(return_file);
        FILE * file_pointer;
        file_pointer = fopen(filename, "r");

        if (file_pointer == NULL) {
                printf("Error: Makefile could not be opened\n");
                exit(1);
        }

        // Target: Reads first word, checks if valid target, stores valid target
        char * temp_target_ptr;
        temp_target_ptr = getTarget(file_pointer);
        // return_file->target = getTarget(file_pointer);;      


        // Dependents: Creates line pointer, stores pointer to each word as dependent
        char * line_ptr;
        line_ptr = malloc(sizeof(char) * MAX_FILE_LEN);
        fgets(line_ptr, MAX_FILE_LEN, file_pointer);

        char * temp_dependent_ptr;

        while (*line_ptr != NULL) {
                temp_dependent_ptr = getDep(line_ptr);
                printf("%c\n", *temp_dependent_ptr);
                // addDependent(getDep(file_pointer));
                while (*(temp_dependent_ptr) != NULL) {
                        temp_dependent_ptr++;
                        line_ptr++;
                        printf("%c\n", *line_ptr);
                }
                line_ptr++;
         }

        fclose(file_pointer);
        return return_file;
}

/**
 * Assumes this is called while fptr is on first column
 * Advances fptr to target and consumes "target:"
 * and returns "target:"
 */
char * getTarget(FILE *fptr) {
        char *target = (char *) mallocWrapper(MAX_FILE_LEN * sizeof(char));
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


char * getCmd(FILE *fptr) {
        char *cmd = (char *) mallocWrapper(MAX_CMD_LEN * sizeof(char));
        fscanf(fptr, "%s", cmd);
        return cmd;
}


// TODO Write fscanf wrapper that takes fscanf output as input and
// does error handling. i.e. scanErr(fscanf(...));

