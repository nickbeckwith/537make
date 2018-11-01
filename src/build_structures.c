#include <stdio.h>
#include "project_tools.h"
#include "build_structures.h"

build_t * buildInit() {
	build_t *build = (build_t *) mallocWrapper(sizeof(build_t));
	build->target = (char *) mallocWrapper(MAX_FILE_LEN * sizeof(char));
	build->dependents = (char **) mallocWrapper(MAX_OTHERS * sizeof(char *));
	build->cmds = (char **) mallocWrapper(MAX_CMD_LEN * sizeof(char *));
	build->cmds_len = 0;
	build->dependents_len = 0;
	return build;
}

build_list_t * buildListInit() {
	build_list_t *build_list = (build_list_t *) mallocWrapper(MAX_OTHERS * sizeof(build_list_t));
	build_list->list = (build_t **) mallocWrapper(MAX_OTHERS * sizeof(build_t *));
	build_list->len = 0;
	return build_list;
}

void buildListFree(build_list_t *list) {
	for (int i = 0; i < list->len; i++) {
		buildFree(*list->list);
		free(list->list[i]);
	}
	free(list->list);
	free(list);
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
	free(build);
}


void addDependent(build_t *build, char *dependent) {
	if (build->dependents_len >= MAX_OTHERS) {
		fprintf(stderr, "Error: Exceeded max size of dependents\n");
		exit(EXIT_FAILURE);
	}
	build->dependents[build->dependents_len] = dependent;
	build->dependents_len++;
}

void addBuild(build_list_t *list, build_t *build) {
	if (list->len >= MAX_OTHERS) {
		fprintf(stderr, "Error: Exceeded max size of build targets\n");
		exit(EXIT_FAILURE);
	}
	list->list[list->len] = build;
	list->len++;
}

void addTarget(build_t *build, char *target) {
	if (target == NULL) {
		fprintf(stderr, "Error: Added target is null");
		exit(EXIT_FAILURE);
	}
	build->target = target;
}

void addCmd(build_t *build, char *cmd) {
	if (build->cmds_len >= MAX_CMD_LEN) {
		fprintf(stderr, "Error: Exceeded max size of commands\n");
		exit(EXIT_FAILURE);
	}
	build->cmds[build->cmds_len] = cmd;
	build->cmds_len++;
}



