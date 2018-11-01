#ifndef P3_PARSER_H
#define P3_PARSER_H

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


void buildListInit(build_list_t *build);

build_list_t * readAll(char *filename);
#endif //P3_PARSER_H
