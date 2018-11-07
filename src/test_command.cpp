#include <unistd.h>
#include <fstream>
#include "catch.hpp"

// include c headers
extern "C" {
	#include "command.h"
	#include <string.h>
}

TEST_CASE("Regular commands are parsed correctly") {
	char cmd[] = "gcc -f try this";
	// automatically passes. Check stdout
	char **argv = tokenizeCommand(cmd);
	REQUIRE(strcmp(argv[0], "gcc") == 0);
	REQUIRE(strcmp(argv[1], "-f") == 0);
	REQUIRE(strcmp(argv[2], "try") == 0);
	REQUIRE(strcmp(argv[3], "this") == 0);
}

TEST_CASE("Tabs and spaces should be handled") {
	char cmd[] = "gcc\t -f    try\tthis";
	// automatically passes. Check stdout
	char **argv = tokenizeCommand(cmd);
	REQUIRE(strcmp(argv[0], "gcc") == 0);
	REQUIRE(strcmp(argv[1], "-f") == 0);
	REQUIRE(strcmp(argv[2], "try") == 0);
	REQUIRE(strcmp(argv[3], "this") == 0);
}

TEST_CASE("argc == 1 should be parsed correctly") {
	char cmd[] = "gcc";
	// automatically passes. Check stdout
	char **argv = tokenizeCommand(cmd);
	REQUIRE(strcmp(argv[0], "gcc") == 0);
}

TEST_CASE("Checks to see if a file is created and deleted in terminal") {
	char cmd[] = "touch .__tmp003";
	runCommand(cmd);
	{
		std::ifstream infile(".__tmp003");
		REQUIRE(infile);
	}
	runCommand("rm .__tmp003");
	std::ifstream infile(".__tmp003");
	REQUIRE(!infile);
}


