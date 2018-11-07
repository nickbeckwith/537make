#include <unistd.h>
#include "main.h"



/**
 * returns 1 if filename exists and 0 if not
 * @param filename
 * @return
 */
int isFile(char *filename) {
	return access(filename, F_OK) == 0;
}