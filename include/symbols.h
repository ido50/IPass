#define MAXUSED 100

struct symbol {
	char *name;
	int defined;
	int type;
	unsigned used[MAXUSED];
	unsigned num_used;
	unsigned entry;
};

struct symbol *new_symbol(char *, int, unsigned, int);

struct symbol *find_symbol(struct queue *, char *);

void update_used(struct symbol *, unsigned);

void update_defined(struct symbol *, int, int);

struct symbol *find_symbol_used_in(struct queue *, unsigned);
struct symbol *find_symbol_defined_in(struct queue *, int);
