#ifndef P3_BUILD_SPEC_H
#define P3_BUILD_SPEC_H

#define MAX_FILE_LEN 255
#define MAX_CMD_LEN  1024     // might change to SC_ARG_MAX
#define MAX_BUF_LEN 1024

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
build_t * buildInit();

/**
 *
 * @param build_list returns pointer to allocated build list
 */
build_list_t * buildListInit();

/**
 *
 * @param build1
 * @param build2
 * @return 1 if build1 and build2 targets are equal
 */
int isBuildEqual(build_t *build1, build_t *build2);

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

/**
 * Replaces current target of build with target specified in parameter
 * @param build
 * @param target
 */
void addTarget(build_t *build, char *target);

#endif //P3_BUILD_SPEC_H
