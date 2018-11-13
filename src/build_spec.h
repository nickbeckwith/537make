// Nick Beckwith, netID: nbeckwith2; Matthew Strimaitis, netID: strimaitis
#ifndef P3_BUILD_SPEC_H
#define P3_BUILD_SPEC_H

#include "list.h"

#define MAX_CMD_LEN  1024     // might change to SC_ARG_MAX
#define MAX_BUF_LEN 1024

typedef struct {
	char * target;         // the target name
	list_t *dependents;          // list of dependents
	list_t *cmds;                // list of commands // can have absurd amount of commands
	int out_of_date;             // 1 if target was out of date, 0 if not, -1 if not set
	int isPhony;                 // 1 if target is phony (no file exists), 0 if not, -1 if not set
} build_t;

/**
 *
 * @param build returns pointer to allocated build
 */
build_t * initBuild();

/**
 *
 * @param build1
 * @param build2
 * @return 1 if build1 and build2 targets are equal
 */
int isBuildEqual(build_t *build1, build_t *build2);

int isBuildEqualByTarget(build_t *build, const char *target);

void freeBuild(build_t *build);

/**
 *
 * @param build file that we want to add to
 * @param dependent dependent to add to list
 */
void addDependent(build_t *build, char *dependent);

/**
 *
 * @param build
 * @param cmd adds command to build
 */
void addCmd(build_t *build, char *cmd);

/**
 * Replaces current target of build with target specified in parameter
 * @param build
 * @param target
 */
void addTarget(build_t *build, char *target);

/**
 *
 * @param build
 * @return 1 if build is phony
 */
int isPhony(build_t *build);
#endif //P3_BUILD_SPEC_H
