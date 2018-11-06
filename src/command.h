#ifndef P3_COMMAND_H
#define P3_COMMAND_H

#include "tools.h"
#include "build_spec.h"

char ** tokenizeCommand(char *cmd);

/**
 * Forks and runs command
 */
void runCommand(char * cmd);
#endif //P3_COMMAND_H
