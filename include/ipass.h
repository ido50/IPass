#define MAXNAME 25
#define MAXMSG 80

struct error *assemble(char *);
void add_error(int, char *);

struct error {
	unsigned int line;
	char *msg;
	struct error *next;
};
