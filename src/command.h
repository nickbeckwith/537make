// Nick Beckwith, netID: nbeckwith2; Matthew Strimaitis, netID: strimaitis
#ifndef P3_COMMAND_H
#define P3_COMMAND_H

#include "tools.h"
#include "build_spec.h"

char ** tokenizeCommand(char *cmd);

/**
 * Forks and runs command
 */
void runCommand(const char * cmd);
#endif //P3_COMMAND_H

/**
 * runs all commands specified in build
 */
void runAllCommands(build_t *build);
