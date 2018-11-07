#include "catch.hpp"
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {
	#include "build_spec.h"
	#include "parser.h"
	#include <string.h>
}
 
TEST_CASE("Test parser basic case") {
	char filename[] = ".__tmp001";
	ofstream file;
	file.open(filename);
	file << "\ndefault: dep1 dep2\n\tgcc -o\t dep1 dep2\n\techo hello\n\n";
	file << "dep1:\n\tgcc -c dep1.c";
	file.close();
	list_t *list = readAll(filename);
	auto *build1 = (build_t *) list->head->data;
	auto *build2 = (build_t *) list->tail->data;
	REQUIRE(strcmp(build1->target, "default") == 0);
	REQUIRE(strcmp((char *)build1->dependents->tail->data, "dep2") == 0);
	REQUIRE(strcmp((char *)build1->cmds->tail->data, "echo hello") == 0);
	REQUIRE(strcmp(build2->target, "dep1") == 0);
	REQUIRE(build2->dependents->head == nullptr);
	REQUIRE(build2->dependents->len == 0);
	remove(filename);
}

TEST_CASE("Test parser screwy case") {
	char filename[] = ".__tmp001";
	ofstream file;
	file.open(filename);
	file << "\n\n\ndefault: dep1 dep2\n\tgcc -o\t dep1 dep2\n\techo hello\n";
	file << "dep1:";
	file.close();
	list_t *list = readAll(filename);
	auto build1 = (build_t *) list->head->data;
	auto *build2 = (build_t *) list->tail->data;
	REQUIRE(strcmp(build1->target, "default") == 0);
	REQUIRE(strcmp((char *)build1->dependents->tail->data, "dep2") == 0);
	REQUIRE(strcmp((char *)build1->cmds->tail->data, "echo hello") == 0);
	REQUIRE(strcmp(build2->target, "dep1") == 0);
	REQUIRE(build2->dependents->head == nullptr);
	REQUIRE(build2->dependents->len == 0);
	remove(filename);
}

TEST_CASE("Case with multiple white space inbetween target and : as well as white space after no dependents") {
	char filename[] = ".__tmp001";
	ofstream file;
	file.open(filename);
	file << "\ndefault \t :dep1 dep2\n\tgcc -o\t dep1 dep2\n\techo hello\n\n";
	file << "dep1: \n\tgcc -c dep1.c";
	file.close();
	list_t *list = readAll(filename);
	auto build1 = (build_t *) list->head->data;
	auto *build2 = (build_t *) list->tail->data;
	REQUIRE(strcmp(build1->target, "default") == 0);
	REQUIRE(strcmp((char *)build1->dependents->tail->data, "dep2") == 0);
	REQUIRE(strcmp((char *)build1->cmds->tail->data, "echo hello") == 0);
	REQUIRE(strcmp(build2->target, "dep1") == 0);
	REQUIRE(strcmp((char *)build2->cmds->head->data, "gcc -c dep1.c") == 0);
	REQUIRE(build2->dependents->head == nullptr);
	REQUIRE(build2->dependents->len == 0);
	remove(filename);
}