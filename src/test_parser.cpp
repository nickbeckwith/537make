#include "catch.hpp"
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {
	#include "build_spec.h"
	#include "parser.h"
}

TEST_CASE("Empty file returns null") {
	char filename[] = ".__tmp001";
	ofstream file;
	file.open(filename);
	file.close();
	REQUIRE(readAll(filename) == nullptr);
	remove(filename);
}
 
TEST_CASE("Test parser basic case") {
	char filename[] = ".__tmp001";
	ofstream file;
	file.open(filename);
	file << "\ndefault: dep1 dep2\n\tgcc -o\t dep1 dep2\n\techo hello\n\n"
	file << "dep1:\n\tgcc -c dep1.c"
	file.close();
	build_list_t *list = readAll(filename);
	REQUIRE(strcmp(list->list[0]->target, "default") == 0);	
	REQUIRE(strcmp(list->list[0]->depenendents[1], "dep2") == 0);	
	REQUIRE(strcmp(list->list[0]->cmds[1], "echo hello") == 0);	
	REQUIRE(strcmp(list->list[1]->target, "dep1") == 0);
	REQUIRE(strcmp(list->list[1]->cmds[0], "gcc -c dep1.c") == 0);	
	REQUIRE(list->list[1]->depenendents == nullptr);
	REQUIRE(list->list[1]->dependents_len == 0);	
	remove(filename);
}

 
 
