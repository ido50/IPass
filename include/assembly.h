#define NUMCMDS 16
#define IMM 0
#define DIR 1
#define INDIR 3
#define DIRREG 4
#define MAXLINE 80
#define MAXLABEL 30
#define MAXNAME 25
#define MAXMSG 80

struct queue *assemble(char *);

struct error {
	char *msg;
	unsigned line;
};

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
