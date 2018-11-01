#ifndef P3_BUILD_SPEC_H
#define P3_BUILD_SPEC_H

typedef struct {
   char * target;       // the target name
   char ** dependents;  // list of dependents
   char ** cmds;        // list of commands
   int dependents_len; // size of dependent list
   int cmds_len;       // size of cmds
} build_t;

typedef struct {
   build_t ** list;
   int len;
} build_list_t;

/**
 *
 * @param build returns pointer to allocated build
 */
void buildInit(build_t *build);

/**
 *
 * @param build_list returns pointer to allocated build list
 */
void buildListInit(build_list_t *build_list);

void buildFree(build_t *build);

void buildListFree(build_list_t *list);

/**
 *
 * @param build file that we want to add to
 * @param dependent dependent to add to list
 */
void addDependent(build_t *build, char *dependent);

/**
 *
 * @param build
 * @param cmd adds commnad to build
 */
void addCmd(build_t *build, char *cmd);

/**
 *
 * @param list
 * @param build adds build target to list
 */
void addBuild(build_list_t *list, build_t *build);
#endif //P3_BUILD_SPEC_H
