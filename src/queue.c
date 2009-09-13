#include <stdlib.h>
#include "queue.h"

/**
 * Creates a new, empty queue
 * 
 * @return	pointer to the new queue
 */
struct queue *new_queue() {
	struct queue *q = (struct queue *)malloc(sizeof(struct queue));
	q->head = NULL;
	q->tail = NULL;
	return q;
}

/**
 * Checks is a queue is empty.
 * 
 * @return	true value if empty, zero otherwise
 */
int is_empty(struct queue *q) {
	if (q->head == NULL && q->tail == NULL)
		return 1;
	return 0;
}

/**
 * Inserts an item to the end of the queue, such as it is the new head
 * 
 * @param q - pointer to the queue where item is inserted
 * @param data - pointer to the new item's data (no need to create the item object yourself)
 * @return	pointer to the item that was pushed
 */
struct item *push(struct queue *q, void *data) {
	struct item *new = _new_item(data, NULL, q->head);

	if (is_empty(q))
		/* this will also be the new tail */
		q->tail = new;
	else {
		/* set the current head to point up to the new one */
		struct item *head = q->head;
		head->next = new;
	}
	q->head = new;

	return new;
}

/**
 * Inserts an item to the start of the queue, such as it is the new tail
 * 
 * @param q - pointer to the queue where item is inserted
 * @param data - pointer to the new item's data (no need to create the item object yourself)
 * @return	pointer to the item that was unshifted
 */
struct item *unshift(struct queue *q, void *data) {
	struct item *new = _new_item(data, q->tail, NULL);

	if (is_empty(q))
		/* this will also be the new tail */
		q->head = new;
	else {
		/* set the current tail to point back to the new one */
		struct item *tail = q->tail;
		tail->prev = new;
	}
	q->tail = new;

	return new;
}

/**
 * Returns the number of items in the queue
 * 
 * @param q - pointer to the queue
 * @return	the number of items in the queue
 */
int queue_length(struct queue *q) {
	struct item *it = q->tail;
	int i = 0;
	while (it != NULL) {
		i++;
		it = it->next;
	}
	return i;
}

/**
 * Removes the first item in a queue and returns it (i.e. removes the tail).
 * 
 * @param q - pointer to the queue
 * @return	the item that was shifted or NULL if queue is empty
 */
struct item *shift(struct queue *q) {
	if (is_empty(q))
		return NULL;
	else {
		struct item *new_tail = q->tail;
		struct item *old_tail = q->tail;

		if (new_tail->next != NULL) {
			/* set the tail's next item as the new tail */
			new_tail = new_tail->next;
			new_tail->prev = NULL;
			q->tail = new_tail;
		} else {
			/* queue is now empty */
			q->tail = NULL;
			q->head = NULL;
		}
		return old_tail;
	}
}

/**
 * Removes the last item in a queue and returns it (i.e. removes the head).
 * 
 * @param q - pointer to the queue
 * @return	the item that was popped or NULL if queue is empty
 */
struct item *pop(struct queue *q) {
	if (is_empty(q))
		return NULL;
	else {
		struct item *new_head = q->head;
		struct item *old_head = q->head;

		if (new_head->prev != NULL) {
			/* set the head's previous item as the new head */
			new_head = new_head->prev;
			new_head->next = NULL;
			q->head = new_head;
		} else {
			/* queue is now empty */
			q->tail = NULL;
			q->head = NULL;
		}
		return old_head;
	}
}

/**
 * Returns the tail of a queue
 * 
 * @param q - pointer to the queue
 * @return	pointer to the queue's tail
 */
struct item *get_tail(struct queue *q) {
	return q->tail;
}

/**
 * Returns the head of a queue
 * 
 * @param q - pointer to the queue
 * @return	pointer to the queue's head
 */
struct item *get_head(struct queue *q) {
	return q->head;
}

/**
 * Removes all items in the queue
 * 
 * @param q - pointer to the queue
 */
void clear_queue(struct queue *q) {
	if (! is_empty(q)) {
		q->head = NULL;
		q->tail = NULL;
	}
}

/**
 * Frees the memory used by the queue
 * 
 * @param q - pointer to the queue
 */
void del_queue(struct queue *q) {
	free(q);
}

/**
 * Creates a new queue item, without pushing it to the queue
 * 
 * @param data - pointer to the data of the item
 * @param next - pointer to the next item the new item will point to
 * @param prev - pointer to the previous item the new item will point to
 * @return	pointer to the new item
 */
struct item *_new_item(void *data, struct item *next, struct item *prev) {
	struct item *new = (struct item *)malloc(sizeof(struct item));
	new->data = data;
	new->next = next;
	new->prev = prev;
	return new;
}
