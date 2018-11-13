// Nick Beckwith, netID: nbeckwith2; Matthew Strimaitis, netID: strimaitis
#ifndef P3_TOOLS_H
#define P3_TOOLS_H

#include <stdlib.h>

void * mallocWrapper(size_t);

/**
 * warning suppression is okay in this case.
 * @param ptr does nothing
 */
void doNothing(__attribute__((unused)) void *ptr);

#endif //P3_TOOLS_H
