#define MAXNAME 25
#define MAXMSG 20

struct error *assemble(char *name);
void add_error(int line, char *msg);

struct error {
	unsigned int line;
	char *msg;
	struct error *next;
};
