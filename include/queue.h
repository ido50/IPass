struct queue {
	struct item *head;
	struct item *tail;
};

struct item {
	void *data;
	struct item *next;
	struct item *prev;
};

struct queue *new_queue();

struct item *unshift(struct queue *, void *);
struct item *push(struct queue *, void *);

struct item *new_item(void *, struct item *, struct item *);
struct item *pop(struct queue *);
struct item *shift(struct queue *);
struct item *get_tail(struct queue *);

int queue_length(struct queue *);
int is_empty(struct queue *);
void clear_queue(struct queue *);
