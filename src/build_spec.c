#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"
#include "build_spec.h"

build_t *buildInit() {
    build_t *build = (build_t *) mallocWrapper(sizeof(build_t));
    build->target = (char *) mallocWrapper(MAX_BUF_LEN * sizeof(char));
    build->dependents = initList();
    build->cmds = initList();
    return build;
}

void buildFree(build_t *build) {
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
	return strcmp(build1->target, build2->target) == 0;
}

