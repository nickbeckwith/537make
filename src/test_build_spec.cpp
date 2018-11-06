#include "catch.hpp"

// include c headers
extern "C" {
	#include "parser.h"
	#include "tools.h"
	#include "build_spec.h"
	#include <string.h>
}

SCENARIO("Build lists can be populated", "[build_list]") {
	GIVEN("An empty build_list") {
		build_list_t *list = buildListInit();
		REQUIRE(list->len == 0);
		char target[] = "main.o";
		char cmd[] = "gcc -c main.c";
		char dependents[] = "main.h";
		build_t *build;
		build = buildInit();
		addBuild(list, build);
		REQUIRE(list->list[0]->dependents_len == 0);
		REQUIRE(list->list[0]->cmds_len == 0);
		REQUIRE(list->len == 1);
		addTarget(list->list[0], target);
		addCmd(list->list[0], cmd);
		addDependent(list->list[0], dependents);
		REQUIRE(list->list[0]->cmds_len == 1);
		REQUIRE(list->list[0]->dependents_len == 1);
		REQUIRE(0 == strcmp(list->list[0]->target, target));
		char cmd1[] = "gcc main.o";
		build_t *build1 = nullptr;
		build1 = buildInit();
		addCmd(build1, cmd1);
		addBuild(list, build1);
		REQUIRE(list->len == 2);
		REQUIRE(list->list[1]->cmds_len == 1);
	}
}


