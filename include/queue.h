/* a bidirectional queue */
struct queue {
	struct item *head;	/* queue's head item */
	struct item *tail;	/* queue's tail item */
};

/* an item in a queue */
struct item {
	void *data;
	struct item *next;
	struct item *prev;
};

/**
 * Creates a new, empty queue
 * 
 * @return	pointer to the new queue
 */
struct queue *new_queue();

/**
 * Inserts an item to the start of the queue, such as it is the new tail
 * 
 * @param q - pointer to the queue where item is inserted
 * @param data - pointer to the new item's data (no need to create the item object yourself)
 * @return	pointer to the item that was unshifted
 */
struct item *unshift(struct queue *, void *);

/**
 * Inserts an item to the end of the queue, such as it is the new head
 * 
 * @param q - pointer to the queue where item is inserted
 * @param data - pointer to the new item's data (no need to create the item object yourself)
 * @return	pointer to the item that was pushed
 */
struct item *push(struct queue *, void *);

/**
 * Creates a new queue item, without pushing it to the queue
 * 
 * @param data - pointer to the data of the item
 * @param next - pointer to the next item the new item will point to
 * @param prev - pointer to the previous item the new item will point to
 * @return	pointer to the new item
 */
struct item *_new_item(void *, struct item *, struct item *);

/**
 * Removes the last item in a queue and returns it (i.e. removes the head).
 * 
 * @param q - pointer to the queue
 * @return	the item that was popped or NULL if queue is empty
 */
struct item *pop(struct queue *);

/**
 * Removes the first item in a queue and returns it (i.e. removes the tail).
 * 
 * @param q - pointer to the queue
 * @return	the item that was shifted or NULL if queue is empty
 */
struct item *shift(struct queue *);

/**
 * Returns the tail of a queue
 * 
 * @param q - pointer to the queue
 * @return	pointer to the queue's tail
 */
struct item *get_tail(struct queue *);

/**
 * Returns the head of a queue
 * 
 * @param q - pointer to the queue
 * @return	pointer to the queue's head
 */
struct item *get_head(struct queue *);

/**
 * Returns the number of items in the queue
 * 
 * @param q - pointer to the queue
 * @return	the number of items in the queue
 */
int queue_length(struct queue *);

/**
 * Checks is a queue is empty.
 * 
 * @return	true value if empty, zero otherwise
 */
int is_empty(struct queue *);

/**
 * Removes all items in the queue
 * 
 * @param q - pointer to the queue
 */
void clear_queue(struct queue *);

/**
 * Frees the memory used by the queue
 * 
 * @param q - pointer to the queue
 */
void del_queue(struct queue *);
