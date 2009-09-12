#include <stdlib.h>
#include "queue.h"

static struct item *head, *tail;

int is_empty() {
	if (head == 0 && tail == 0)
		return 1;
	return 0;
}

struct item *new_item(void *data, struct item *next, struct item *prev) {
	struct item *new = (struct item *)malloc(sizeof(struct item));
	new->data = data;
	new->next = next;
	new->prev = prev;
	return new;
}

void push(void *data) {
	struct item *new = new_item( data, 0, head );
	if (is_empty())
		tail = new;
	else
		head->next = new;
	head = new;
}

void unshift(void *data) {
	struct item *new = new_item( data, tail, 0 );
	if (is_empty())
		head = new;
	else
		tail->prev = new;
	tail = new;
}

struct item *shift() {
	if (is_empty())
		return 0;
	else {
		struct item *new_tail = tail->next;
		struct item *old_tail = tail;
		new_tail->prev = 0;
		tail = new_tail;
		return old_tail;
	}
}

struct item *pop() {
	if (is_empty())
		return 0;
	else {
		struct item *new_head = head->prev;
		struct item *old_head = head;
		new_head->next = 0;
		head = new_head;
		return old_head;
	}
}

void clear_queue() {
	if (is_empty() == 0) {
		head = 0;
		tail = 0;
	}
}
