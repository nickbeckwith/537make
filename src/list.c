// Nick Beckwith, netID: nbeckwith2; Matthew Strimaitis, netID: strimaitis
#include "list.h"
#include "tools.h"
#include "graph.h"

// data must by dynamically allocated
node_t *initNode(void *data, void (*free_ptr)(void *)) {
	node_t *node = (node_t *) mallocWrapper(sizeof(node_t));
	node->data = data;
	node->free_ptr = free_ptr;
	node->next = NULL;
	return node;
}

void freeNode(node_t *node) {
	if (node == NULL) {
		return;
	}
	node->free_ptr(node->data);
	free(node);
}


list_t *initList() {
	list_t *list = (list_t *) mallocWrapper(sizeof(list_t));
	list->head = NULL;
	list->len = 0;
	list->tail = NULL;
	return list;
}

// adds to tail in order to keep order where head is least recently added
void addElem(list_t *list, void *data, void (*free_ptr)(void *)) {
	node_t *node = initNode(data, free_ptr);
	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		list->tail->next = node;
		list->tail = node;
	}
	list->len++;
}


void freeList(list_t *list) {
	node_t *temp;
	node_t *head = list->head;
	while (head != NULL) {
		temp = head;
		head = head->next;
		freeNode(temp);
	}
	free(list);
}

