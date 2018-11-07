#ifndef P3_LIST_H
#define P3_LIST_H

typedef struct node_t {
	void *data;
	struct node_t *next;
} node_t;

typedef struct list_t {
	node_t *head;
	node_t *tail;
	int len;
} list_t;

/**
 * initializes list by allocating memory
 * @return list that is allocated
 */
list_t * initList();

/**
 * frees in a shallow way. Only frees what was allocated in init
 * and what was allocated in initNode
 * @param list
 */
void freeList(list_t *list);

/**
 * Adds element to end of list
 * @param list target list
 * @param data pointer to any data
 */
void addElem(list_t *list, void *data);
#endif //P3_LIST_H
