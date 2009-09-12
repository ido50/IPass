#define NUMCMDS 16
#define IMM 00
#define DIR 01
#define INDIR 03
#define DIRREG 04
#define MAXLINE 80
#define CMD_LINE 1
#define INST_LINE 2
#define CMD_STATE 2
#define DATA_STATE 3

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
void parse_line(char *, int);
int parse_word(char *);
int makes_sense(unsigned short, unsigned short, unsigned short);
int legal_word(char *, unsigned short);
char *state_name(unsigned short);

int is_num(char *);
int is_label(char *);
int is_string(char *);
int is_cmd(char *);

extern struct command commands[NUMCMDS];
