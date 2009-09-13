#define NUMCMDS 16		/* number of commands defined by the language */
#define IMM 0			/* immediate addressing */
#define DIR 1			/* direct addressing */
#define INDIR 3			/* indirect addressing */
#define DIRREG 4		/* direct register addressing */
#define MAXLINE 80		/* maximum length of a line in the source file */
#define MAXLABEL 30		/* maximum length of a label name */
#define MAXNAME 40		/* maximum length of a source file name */
#define MAXMSG 80		/* maximum length of an error message */

/**
 * Assembles an assembly file represented by a name
 * 
 * @param name - the name of the file (without the ".as" extension)
 * @return	pointer to the errors queue (even if no errors where encountered)
 */
struct queue *assemble(char *);

/* an error message */
struct error {
	char *msg;		/* the error message */
	unsigned line;		/* the line in the source file that invoked it */
};

/* a standard assembly command */
struct command {
	char name[3];			/* the command's name */
	unsigned code;			/* the command's code */
	unsigned short src_op;		/* true if this command expects a source operand */
	unsigned short dest_op;		/* true if this command expects a destination operand */
	unsigned short src_imm;		/* true if accepts immediate addressing in the source */
	unsigned short src_dir;		/* true if accepts direct addressing in the source */
	unsigned short src_indir;	/* true if accepts indirect addressing in the source */
	unsigned short src_dirreg;	/* true if accepts direct register addressing in the source */
	unsigned short dest_imm;	/* true if accepts immediate addressing in the destination */
	unsigned short dest_dir;	/* true if accepts direct addressing in the destination */
	unsigned short dest_indir;	/* true if accepts indirect addressing in the destination */
	unsigned short dest_dirreg;	/* true if accepts direct register addressing in the destination */
};

/**
 * Finds and returns a command in the command list according to name
 * 
 * @param name - the name of the command
 * @return	the command structure if found, NULL otherwise
 */
struct command *find_cmd(char *);

void _break_line(char *);
void _parse_words();
int legal_word(char *, unsigned short);

int is_num(char *);
int is_inum(char *);
int is_label(char *);
int is_ilabel(char *);
int is_string(char *);
int is_data_inst(char *);
int is_string_inst(char *);
int is_comma(char *);
int is_cmd(char *);
int legal_cmd(char *);
int legal_label(char *);
int is_entry(char *);
int is_extern(char *);
int is_register(char *);
int legal_register(char *);
char *label_name(char *);
int op_type(char *name);

extern struct command commands[NUMCMDS];

void _update_syms();
void _print_syms(char *);
void _print_ob(char *);

void _treat_label(char *, unsigned, int);
int _allowed(struct command *, char *, int);
char *_get_label();
char *_address_name(int);
void _print_commands();
void _add_error(char *, ...);
