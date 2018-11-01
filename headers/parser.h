struct build_t {
   char ** target;
   char ** dependents;
   char ** cmds;
   int dependents_size;
   int cmds_size;
};

struct build_list_t {
   build_t ** list;
   int list_len;
};


make_t * readAll(char *filename);
