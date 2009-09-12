#define NUMCMDS 16
#define IMM 00
#define DIR 01
#define INDIR 03
#define DIRREG 04
#define MAXLINE 80
#define MAXLABEL 30

struct command {
	char name[3];
	unsigned short code;
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
void break_line(char *, int);
void parse_line(int);
int parse_word(char *);
int legal_word(char *, unsigned short);

int is_num(char *);
int is_label(char *);
int is_string(char *);
int is_data_inst(char *);
int is_string_inst(char *);
int is_comma(char *);
int is_cmd(char *);
int legal_cmd(char *);
int legal_label(char *);
char *label_name(char *);

extern struct command commands[NUMCMDS];

void print_queue();
