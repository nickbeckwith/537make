#ifndef P3_LIST_H
#define P3_LIST_H

enum type_e{char_arr_e, vertex_e, build_e, default_e};

typedef struct node_t {
	void *data;
	void (*free_ptr)(void *);       // contains pointer to function that frees data
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
 * frees list and data members if data members
 * is of type char arr, vertex or build
 * @param list
 */
void freeList(list_t *list);

/**
 * Adds element to end of list
 * @param list target list
 * @param data pointer to any data
 * @param free_ptr function that frees data
 */
void addElem(list_t *list, void *data, void (*free_ptr)(void *));
#endif //P3_LIST_H
