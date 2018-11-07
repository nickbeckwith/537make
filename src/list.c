#include "list.h"
#include "tools.h"

node_t * initNode(void *data) {
	node_t *node = (node_t *) mallocWrapper(sizeof(node_t));
	node->data = data;
	node->next = NULL;
	return node;
}


list_t *initList() {
	list_t *list = (list_t *) mallocWrapper(sizeof(list_t));
	list->head = NULL;
	list->len = 0;
	list->tail = NULL;
	return list;
}

// adds to tail in order to keep order where head is least recently added
void addElem(list_t *list, void *data) {
	node_t *node = initNode(data);
	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	list->len++;
}

/**
 * recursively frees node
 * and only node (not data)
 * @param node
 */
void freeListHelper(node_t *node) {
	if (node == NULL) {
		return;
	} else {
		freeListHelper(node->next);     // need to start from the back of list
		free(node);
	}
}

void freeList(list_t *list) {
	freeListHelper(list->head);
	free(list);
}

