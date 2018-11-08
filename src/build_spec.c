#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "build_spec.h"

build_t *initBuild() {
    build_t *build = (build_t *) mallocWrapper(sizeof(build_t));
    build->target = (char *) mallocWrapper(MAX_BUF_LEN * sizeof(char));
    build->dependents = initList();
    build->cmds = initList();
    build->isPhony = -1;
    build->out_of_date = -1;
    return build;
}

void freeBuild(build_t *build) {
	free(build->target);
	freeList(build->dependents);
	freeList(build->cmds);
	free(build);
}

void addDependent(build_t *build, char *dependent) {
	addElem(build->dependents, dependent);
}

void addTarget(build_t *build, char *target) {
        build->target = target;
}

void addCmd(build_t *build, char *cmd) {
	addElem(build->cmds, cmd);
}

int isBuildEqual(build_t *build1, build_t *build2) {
	if (build1->target == NULL || build2->target == NULL) {
		return 0;
	}
	return strcmp(build1->target, build2->target) == 0;
}

int isBuildEqualByTarget(build_t *build, const char *target) {
	if (build->target == NULL || target == NULL) {
		return 0;
	}
	return strcmp(build->target, target) == 0;
}

int isPhony(build_t *build) {
	return build->isPhony;
}

