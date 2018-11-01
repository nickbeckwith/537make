struct build_t {
   char * target;       // the target name
   char ** dependents;  // list of dependents
   char ** cmds;        // list of commands
   int dependents_size; // size of dependent list
   int cmds_size;       // size of cmds
};

struct build_list_t {
   build_t ** list;
   int list_len;
};

build_list_t * readAll(char *filename);

// private methods that would help
void build_init(build_t *build);
void build_list_init(build_list_t *build_list);
void add_dependent(build_t *build, char *dependent);
void add_cmd(build_t *build, char *cmd);
void add_build(build_list_t *build_list, build_t build);
