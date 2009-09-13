struct codeln {
	int opcode;
	int src_t;
	int src_reg;
	int dest_t;
	int dest_reg;
	int num;
	int imm;
	char type;
};

struct codeln *new_codeln(int);

struct dataln {
	int num;
	int data;
};

struct dataln *new_dataln(int, int);

struct codeln *find_codeln(int);

void _get_operand(struct codeln *, struct command *, char *, int);
void _add_register(struct codeln *, char *, int);
void _add_type(struct codeln *, char *, int);
