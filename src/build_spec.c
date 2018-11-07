#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "build_spec.h"

void buildFree(build_t *build);

build_t *buildInit() {
        build_t *build = (build_t *) mallocWrapper(sizeof(build_t));
        build->target = (char *) mallocWrapper(MAX_BUF_LEN * sizeof(char));
        build->dependents = (char **) mallocWrapper(MAX_BUF_LEN * sizeof(char *));
        build->cmds = (char **) mallocWrapper(MAX_BUF_LEN * sizeof(char *));
        build->cmds_len = 0;
        build->dependents_len = 0;
}

build_list_t *buildListInit() {
        build_list_t *build_list = (build_list_t *) mallocWrapper(sizeof(build_list_t));
        build_list->list = mallocWrapper(MAX_BUF_LEN * sizeof(build_t *));
        build_list->len = 0;
}

void buildListFree(build_list_t *list) {
        for (int i = 0; i < list->len; i++) {
                buildFree(*list->list);
                free(list->list[i]);
        }
        free(list->list);
}

void addDependent(build_t *build, char *dependent) {
        if (build->dependents_len >= MAX_BUF_LEN) {
                fprintf(stderr, "Error: Exceeded max size of dependents\n");
                exit(EXIT_FAILURE);
        }
        build->dependents[build->dependents_len] = dependent;
        // Increments dependents length
        build->dependents_len++;
}

void addTarget(build_t *build, char *target) {
        build->target = target;
}

void addBuild(build_list_t *list, build_t *build) {
        if (list->len >= MAX_BUF_LEN) {
                fprintf(stderr, "Error: Exceeded max size of build targets\n");
                exit(EXIT_FAILURE);
        }
        list->list[list->len] = build;
        list->len++;
}

void addCmd(build_t *build, char *cmd) {
        if (build->cmds_len >= MAX_BUF_LEN) {
                fprintf(stderr, "Error: Exceeded max size of commands\n");
                exit(EXIT_FAILURE);
        }
        build->cmds[build->cmds_len] = cmd;
        // Increments commands length
        build->cmds_len++;
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
