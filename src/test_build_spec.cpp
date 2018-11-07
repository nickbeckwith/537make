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
		list_t *list = initList();
		REQUIRE(list->len == 0);
		char target[] = "main.o";
		char cmd[] = "gcc -c main.c";
		char dependents[] = "main.h";
		build_t *build;
		build = buildInit();
		addElem(list, build);
		build_t *build1 = (build_t *) list->head->data;
		REQUIRE(build1->dependents->len == 0);
		REQUIRE(build1->cmds->len == 0);
		REQUIRE(list->len == 1);
		addTarget(build1, target);
		addCmd(build1, cmd);
		addDependent(build1, dependents);
		REQUIRE(build1->cmds->len == 1);
		REQUIRE(build1->dependents->len == 1);
		REQUIRE(0 == strcmp(build1->target, target));
		char cmd1[] = "gcc main.o";
		build_t *build2 = buildInit();
		addElem(list, build2);
		build2 = (build_t *) list->tail->data;
		addCmd(build2, cmd1);
		REQUIRE(list->len == 2);
		REQUIRE(build2->cmds->len == 1);
	}
}


