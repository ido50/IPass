struct item {
	void *data;
	struct item *next;
	struct item *prev;
};

struct item *unshift(void *data);
struct item *push(void *data);

struct item *new(void *, struct item *, struct item *);
struct item *pop();
struct item *shift();
struct item *get_tail();

int is_empty();
void clear_queue();
