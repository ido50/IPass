#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct queue *new_queue() {
	struct queue *q = (struct queue *)malloc(sizeof(struct queue));
	q->head = NULL;
	q->tail = NULL;
	return q;
}

int is_empty(struct queue *q) {
	if (q->head == NULL && q->tail == NULL)
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

struct item *push(struct queue *q, void *data) {
	struct item *new = new_item(data, NULL, q->head);

	if (is_empty(q))
		q->tail = new;
	else {
		struct item *head = q->head;
		head->next = new;
	}
	q->head = new;

	return new;
}

struct item *unshift(struct queue *q, void *data) {
	struct item *new = new_item(data, q->tail, NULL);

	if (is_empty(q))
		q->head = new;
	else {
		struct item *tail = q->tail;
		tail->prev = new;
	}
	q->tail = new;

	return new;
}

int queue_length(struct queue *q) {
	struct item *it = q->tail;
	int i = 0;
	while (it != NULL) {
		i++;
		it = it->next;
	}
	return i;
}

struct item *shift(struct queue *q) {
	if (is_empty(q))
		return NULL;
	else {
		struct item *new_tail = q->tail;
		struct item *old_tail = q->tail;

		if (new_tail->next != NULL) {
			new_tail = new_tail->next;
			new_tail->prev = NULL;
			q->tail = new_tail;
		} else {
			q->tail = NULL;
			q->head = NULL;
		}
		return old_tail;
	}
}

struct item *pop(struct queue *q) {
	if (is_empty(q))
		return NULL;
	else {
		struct item *new_head = q->head;
		struct item *old_head = q->head;

		if (new_head->prev != NULL) {
			new_head = new_head->prev;
			new_head->next = NULL;
			q->head = new_head;
		} else {
			q->tail = NULL;
			q->head = NULL;
		}
		return old_head;
	}
}

struct item *get_tail(struct queue *q) {
	return q->tail;
}

struct item *get_head(struct queue *q) {
	return q->head;
}

void clear_queue(struct queue *q) {
	if (! is_empty(q)) {
		q->head = NULL;
		q->tail = NULL;
	}
}

void del_queue(struct queue *q) {
	free(q);
}
