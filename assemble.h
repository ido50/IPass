#define MAXNAME 25
#define MAXMSG 20

struct error *assemble(char *name);
void add_error(struct error *err);

struct error {
	unsigned int line;
	char *msg;
	struct error *next;
};
