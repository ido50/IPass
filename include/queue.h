struct item {
	void *data;
	struct item *next;
	struct item *prev;
};

void unshift(void *data);
void push(void *data);

struct item *new(void *, struct item *, struct item *);
struct item *pop();
struct item *shift();

int is_empty();
void clear_queue();
