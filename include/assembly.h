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
void add_error(char *, ...);

struct error {
	char *msg;
	unsigned line;
};

struct command {
	char name[3];
	unsigned code;
	unsigned short src_op;
	unsigned short dest_op;
	unsigned short src_imm;
	unsigned short src_dir;
	unsigned short src_indir;
	unsigned short src_dirreg;
	unsigned short dest_imm;
	unsigned short dest_dir;
	unsigned short dest_indir;
	unsigned short dest_dirreg;
};

void init_cmds();
struct command *find_cmd(char *);
void break_line(char *);
void parse_line();
int parse_word(char *);
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

void update_syms();
void print_syms(char *);
void print_ob(char *);

void _treat_label(char *, unsigned, int);
int allowed(struct command *, char *, int);
char *_get_label();
char *address_name(int);
void print_commands();
