#include <stdio.h>
#include "parser.h"
#include "build_spec.h"
#include "tools.h"

build_list_t * readAll(char *filename) {
        build_list_t * return_file;
        FILE * file_pointer;
        // char * temp_char_ptr
        return_file->target = getTarget(file_pointer);

        while(getCmd(file_pointer) != NULL) {
                ((return_file->cmds)* + return_file->cmds_len)* = getCmd(file_pointer);
                // MAY NOT NEED TO INCLUDE, DEPENDING OTHER FUNCTIONS
                return_file->cmds_len++;
        }

        while(getDep(file_pointer) != NULL) {
                ((return_file->dependents)* + return_file->dependents_len)* = getDep(file_pointer);
                // MAY NOT NEED TO INCLUDE, DEPENDING OTHER FUNCTIONS
                return_file->dependents_len++;
        }


}

/**
 * Assumes this is called while fptr is on first column
 * Advances fptr to target and consumes "target:"
 * and returns "target"
 */
char * getTarget(FILE *fptr) {
        char *target = (char *) mallocWrapper(MAX_FILE_LEN * sizeof(char));
        fscanf(fptr, "%s:", target);
        return target;
        //TODO ensure that fscanf only gets target if target is on first column
}

char * getDep(FILE *fptr) {
        char *cmd = (char *) mallocWrapper(MAX_OTHERS_LEN * sizeof(char));
        fscanf(fptr, "%s", cmd);
        return cmd;
}


char * getCmd(FILE *fptr) {
        char *cmd = (char *) mallocWrapper(MAX_CMD_LEN * sizeof(char));
        fscanf(fptr, "%s", cmd);
        return cmd;
}


// TODO Write fscanf wrapper that takes fscanf output as input and
// does error handling. i.e. scanErr(fscanf(...));
