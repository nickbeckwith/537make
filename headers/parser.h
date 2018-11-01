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


make_t * readAll(char *filename);

// private methods that would help
void add_dependent(build_t build, char * dependent);
void add_cmd(build_t build, char * cmd);
