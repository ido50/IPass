/* a code line (that will be printed into the output object file */
struct codeln {
	int opcode;		/* the line's opcode (or complete code if not a command line) */
	int src_t;		/* the line's source type */
	int src_reg;		/* the line's source register */
	int dest_t;		/* the line's destination type */
	int dest_reg;		/* the line's destination register */
	int num;		/* the line's number in the output file */
	int imm;		/* true if line represents an immediate value */
	char type;		/* the type of the line ('a' for absolute, 'e' for external and 'r' for relocatable) */
};

/* a data line (that will be printed into the output object file */
struct dataln {
	int num;		/* the number of the line in the output file */
	int data;		/* the value of the line */
};

struct codeln *_new_codeln(int);
struct dataln *_new_dataln(int, int);
struct codeln *_find_codeln(int);

void _get_operand(struct codeln *, struct command *, char *, int);
void _add_register(struct codeln *, char *, int);
void _add_type(struct codeln *, char *, int);
