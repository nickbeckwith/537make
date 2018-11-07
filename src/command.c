#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "command.h"

char ** tokenizeCommand(char *cmd) {
	char **argv = (char **) mallocWrapper(MAX_CMD_LEN * sizeof(char *));
	int argc = 0;
	argv[argc] = strtok(cmd, " \t");     // initialize and pass the first token
	while (argv[argc] != NULL) {
		argc++;
		argv[argc] = strtok(NULL, " \t");
	}
	return argv;
}

void runCommand(const char *cmd) {
	char cmd_mutable[MAX_BUF_LEN];
	strcpy(cmd_mutable, cmd);
	char **tokens = tokenizeCommand(cmd_mutable);
	char *filename = *tokens;       // first token
	char **argv = tokens;
	pid_t pid = fork();
	int status;
	if (pid == 0) {
		// child process
		if (execvp(filename, argv) == -1) {
			perror("Error: ");
			exit(EXIT_FAILURE);
		}
	} else if (pid < 0) {
		perror("Error: ");
		exit(EXIT_FAILURE);
	} else {
		// parent process
		if ((pid = wait(&status)) == -1) {
			perror("Error: ");
			exit(EXIT_FAILURE);
		}
	}
}