struct queue {
	struct item *head;
	struct item *tail;
};

struct item {
	void *pval;
	int ival;
	struct item *next;
	struct item *prev;
};

struct queue *new_queue();

struct item *unshift(struct queue *, void *);
struct item *push(struct queue *, void *);
struct item *unshift_int(struct queue *, int);
struct item *push_int(struct queue *, int);

struct item *new_item(void *, struct item *, struct item *);
struct item *new_int_item(int, struct item *, struct item *);
struct item *pop(struct queue *);
struct item *shift(struct queue *);
struct item *get_tail(struct queue *);
struct item *get_head(struct queue *);

int queue_length(struct queue *);
int is_empty(struct queue *);
void clear_queue(struct queue *);
