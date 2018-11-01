#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "build_spec.h"

#define MAX_FILE_LEN 255
#define MAX_CMD_LEN  1024     // might change to SC_ARG_MAX
#define MAX_OTHERS  1024

/**
 *
 * @param build returns pointer to allocated build
 */
void buildInit(build_t *build) {
	build = (build_t *) mallocWrapper(sizeof(build_t));
	build->target = (char *) mallocWrapper(MAX_FILE_LEN * sizeof(char));
	build->dependents = (char **) mallocWrapper(MAX_OTHERS * sizeof(char *));
	build->cmds = (char **) mallocWrapper(MAX_CMD_LEN * sizeof(char *));
	build->cmds_len = 0;
	build->dependents_len = 0;
}

/**
 *
 * @param build_list returns pointer to allocated build list
 */
void buildListInit(build_list_t *build_list) {
	build_list = (build_list_t *) mallocWrapper(sizeof(build_list_t));
	build_list->list = mallocWrapper(MAX_OTHERS * sizeof(build_t *));
	build_list->len = 0;
}

void buildFree(build_t *build) {
	free(build->target);
	for (int i = 0; i < build->dependents_len; i++) {
		free(build->dependents[i]);
	}
	free(build->dependents);
	for (int i = 0; i < build->cmds_len; i++) {
		free(build->cmds[i]);
	}
	free(build->cmds);
}

void buildListFree(build_list_t *list) {
	for (int i = 0; i < list->len; i++) {
		buildFree(*list->list);
		free(list->list[i]);
	}
	free(list->list);
}

/**
 *
 * @param build file that we want to add to
 * @param dependent dependent to add to list
 */
void addDependent(build_t *build, char *dependent) {
	if (build->dependents_len >= MAX_OTHERS) {
		fprintf(stderr, "Error: Exceeded max size of dependents\n");
		exit(EXIT_FAILURE);
	}
	build->dependents[build->dependents_len] = dependent;
}

/**
 *
 * @param build
 * @param cmd adds commnad to build
 */
void addCmd(build_t *build, char *cmd) {
	if (build->cmds_len >= MAX_CMD_LEN) {
		fprintf(stderr, "Error: Exceeded max size of commands\n");
		exit(EXIT_FAILURE);
	}
	build->cmds[build->cmds_len] = cmd;
}

/**
 *
 * @param list
 * @param build adds build target to list
 */
void addBuild(build_list_t *list, build_t build) {
	if (list->len >= MAX_OTHERS) {
		fprintf(stderr, "Error: Exceeded max size of build targets\n");
		exit(EXIT_FAILURE);
	}
	list->list[list->list] = build;
}

