#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "ipass.h"
#include "assembly.h"
#include "queue.h"
#include "symbols.h"
#include "lines.h"

static struct error *tail;
static struct error *head;

static struct queue *wordq;
static struct queue *cmdq;
static struct queue *dataq;
static struct queue *entq;
static struct queue *extq;
static struct queue *symt;

static unsigned ic, dc, lnum;

struct error *assemble(char *name) {
	FILE *fp;			/* source file handler */
	char *source = (char *)malloc(MAXNAME);

	/* append .as to the source file name */
	strcpy(source, name);
	strcat(source, ".as");

	/* check this source file exists */
	if ((fp = fopen(source, "r")) == NULL)
		add_error(0, "ca't open ", source, "end");
	else {
		char *line = (char *)malloc(MAXLINE);

		wordq = new_queue();
		cmdq = new_queue();
		dataq = new_queue();
		entq = new_queue();
		extq = new_queue();
		symt = new_table();
		ic = 0;
		dc = 0;
		lnum = 0;
		while (fgets(line, MAXLINE, fp) != NULL) {
			lnum++;
			if (*line != ';')
				break_line(line);
		}
		fclose(fp);

		/*update_syms();
		print_cmdq();
		print_dataq();*/
	}

	return tail;
}

void break_line(char *line) {
	clear_queue(wordq);
	while (*line != '\0') {
		char *word = (char *)malloc(MAXLINE);
		if (*line == ',') {
			strncat(word, line, 1);
			line++;
		} else {
			while (isspace(*line) == 0 && *line != ',') {
				strncat(word, line, 1);
				line++;
			}
		}
		if (strlen(word) > 0)
			push(wordq, word);
		else
			line++;
	}
	parse_line();
}

char *_get_label() {
	struct item *it = shift(wordq);
	char *word = (char *)it->pval;
	char *label = (char *)malloc(MAXLABEL);

	if (is_label(word)) {
		label = label_name(word);
		if (legal_label(label) == 0) {
			add_error(label, " is not a valid label.", "end");
			label = NULL;
		}
	} else {
		label = NULL;
		unshift(wordq, word);
	}
	return label;
}

void _add_type(struct codeln *cmdln, char *srcdst, int type) {
	if (strcmp(srcdst, "src") == 0)
		cmdln->src_t = type;
	else
		cmdln->dest_t = type;
}

int allowed(struct command *cmd, char *srcdst, int type) {
	if (strcmp(srcdst, "src") == 0) {
		if (type == IMM)
			return cmd->src_imm;
		else if (type == DIR)
			return cmd->src_dir;
		else if (type == INDIR)
			return cmd->src_indir;
		else
			return cmd->src_dirreg;
	} else {
		if (type == IMM)
			return cmd->dest_imm;
		else if (type == DIR)
			return cmd->dest_dir;
		else if (type == INDIR)
			return cmd->dest_indir;
		else
			return cmd->dest_dirreg;
	}
}
		

void _add_register(struct codeln *cmdln, char *srcdst, int name) {
	if (strcmp(srcdst, "src") == 0)
		cmdln->src_reg = name;
	else
		cmdln->dest_reg = name;
}

char *address_name(int type) {
	if (type == IMM)
		return "immediate";
	else if (type == DIR)
		return "direct";
	else if (type == INDIR)
		return "indirect";
	else
		return "direct register";
}

void _get_operand(struct codeln *cmdln, struct command *cmd, char *srcdst, int expect_comma) {
	if (is_empty(wordq))
		/* no operand */
		add_error(srcdst, " operand is missing", "end");
	else {
		if (expect_comma) {
			struct item *opword = shift(wordq);
			if (! is_comma((char *)opword->pval)) {
				unshift(wordq, opword);
				add_error("missing comma between operands", "end");
			}
		}

		if (is_empty(wordq))
			add_error(srcdst, " operand is missing", "end");
		else {
			int type;
			struct item *opword = shift(wordq);
			char *name = (char *)opword->pval;
			type = op_type(name);

			_add_type(cmdln, srcdst, type);

			if (! allowed(cmd, srcdst, type))
				add_error(cmd->name, " does not allow ", address_name(type), " addressing", "end");

			if (type == DIR || type == INDIR) {
				struct codeln *opln = new_codeln(0);
				struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
				opln->imm = 1;

				if (type == INDIR)
					/* increase to get rid of the @ sign */
					name++;

				if ((sym = find_symbol(symt, name)) != NULL)
					update_used(sym, opln->num);
				else
					push(symt, new_symbol(name, -1, opln->num, 0));
				push(cmdq, opln);
			} else if (type == IMM) {
				struct codeln *opln = new_codeln(0);
				opln->imm = 1;

				/* increase to get rid of the # sign */
				opln->opcode = atoi(++name);

				push(cmdq, opln);
			} else {
				/* DIRREG */
				/* increase to get rid of the 'r' char */
				_add_register(cmdln, srcdst, atoi(++name));
			}
		}
	}
}

void _treat_label(char *label, unsigned num, int type) {
	if (label != NULL) {
		struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
		if ((sym = find_symbol(symt, label)) != NULL) {
			if (sym->defined >= 0)
				add_error(label, " was already defined", "end");
			else
				update_defined(sym, num, type);
		} else
			push(symt, new_symbol(label, num, -1, type));
	}
}

void parse_line() {
	char *label = _get_label(lnum);

	if (is_empty(wordq))
		add_error("void line", "end");
	else {
		struct item *next = shift(wordq);
		char *word = (char *)next->pval;
		if (is_cmd(word)) {
			/* check cmd name is legal */
			if (legal_cmd(word) == 0)
				add_error(word, " is not a valid command", "end");
			else {
				/* get operands and check that they are legal */
				/* do something with the label */
				int expect_comma = 0;
				struct command *cmd = find_cmd(word);
				struct codeln *cmdln = new_codeln(cmd->code);

				push(cmdq, cmdln);

				if (cmd->src_op && cmd->dest_op)
					expect_comma = 1;

				if (cmd->src_op) {
					_get_operand(cmdln, cmd, "src", 0);
				}
				if (cmd->dest_op) {
					_get_operand(cmdln, cmd, "dest", expect_comma);
				}

				_treat_label(label, cmdln->num, 0);
			}
		} else if (is_data_inst(word)) {
			/* get data and check that it's legal */
			int i = 0;

			if (is_empty(wordq))
				add_error("no data given", "end");
			else {
				int expect_num = 1;
				int expect_comma = 0;

				while (! is_empty(wordq)) {
					struct item *next = shift(wordq);
					char *data = (char *)next->pval;

					if (expect_num && ! is_num(data))
						add_error(data, " is not a valid number", "end");
					else if (expect_num) {
						i++;
						/* do something with this number */
						push(dataq, new_dataln(atoi(data), dc + i));
						expect_comma = 1;
						expect_num = 0;
					} else if (expect_comma && ! is_comma(data)) {
						/* check if there are no more words */
						if (! is_empty(wordq)) {
							add_error("i was expecting a comma or end of line", "end");
							expect_num = 0;
							expect_comma = 0;
						}
					} else if (expect_comma) {
						expect_num = 1;
						expect_comma = 0;
						if (is_empty(wordq))
							add_error("no number given after comma", "end");
					}
				}
			}

			if (i > 0)
				_treat_label(label, dc + 1, 1);
			dc += i;
		} else if (is_string_inst(word)) {
			/* get string and check that it's legal */
			struct item *next = shift(wordq);
			int i = 0;

			if (next == NULL)
				add_error("no string given", "end");
			else {
				char *string = (char *)next->pval;
				if (is_string(string)) {
					/* remove the '"' signs from the string */
					char *str = (char *)malloc(MAXLINE);

					string++;
					strncpy(str, string, strlen(string)-1);
					/* code that string */
					while (*str != '\0') {
						i++;
						push(dataq, new_dataln(*str, dc+i));
						str++;
					}
					push(dataq, new_dataln(0, dc + ++i));
				} else
					add_error(string, " is not a valid string", "end");
			}

			if (i > 0)
				_treat_label(label, dc + 1, 1);
			dc += i;
		} else if (is_entry(word) || is_extern(word)) {
			/* get a label and check it exists */
			struct item *lbl = shift(wordq);
			if (lbl == NULL)
				add_error("missing label name", "end");
			else {
				char *label = (char *)lbl->pval;
				if (is_entry(word))
					push(entq, label);
				else
					push(extq, label);
			}
		}
	}
}

void add_error(char *msg1, ...) {
	char *i = (char *)malloc(MAXMSG);
	char *msg = (char *)malloc(MAXMSG);
	struct error *err = (struct error *)malloc(sizeof(struct error));
	va_list ap;

	err->line = lnum;
	err->next = NULL;

	if (head == NULL) {
		head = err;
		tail = err;
	} else {
		head->next = err;
		head = err;
	}

	/* initialize ptr to point to the first argument after the format string */
	va_start(ap, msg1);

	for (i = msg1; strcmp(i, "end") != 0; i = va_arg(ap, char *))
		strcat(msg, i);
	va_end(ap);

	err->msg = msg;
}

void update_syms() {
	struct item *tail = get_tail(symt);
	struct item *extail = get_tail(extq);
	struct item *entail = get_tail(entq);

	/* update external symbols */
	printf("source.ext\n---------------------\n");
	while (extail != NULL) {
		char *label = (char *)extail->pval;
		struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
		if ((sym = find_symbol(symt, label)) != NULL) {
			struct item *head = get_head(sym->used);
			printf("I am being used on %d\n", head->ival);
			/*printf("%s %o\n", sym->name, sym->used[0]);*/
		}
		extail = extail->next;
	}

	/* update all symbols */
	while (tail != NULL) {
		struct symbol *sym = (struct symbol *)tail->pval;
		if (sym->type == 1)
			/* it's a data symbol */
			sym->defined += ic - 1;
		/*if (! is_empty(sym->used)) {
			struct codeln *line = find_codeln(sym->used[0]);
			line->opcode = sym->defined;
		/*}*/
		tail = tail->next;
	}

	/* print entries */
	printf("source.ent\n---------------------\n");
	while (entail != NULL) {
		char *label = (char *)entail->pval;
		struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
		if ((sym = find_symbol(symt, label)) != NULL)
			printf("%s %o\n", sym->name, sym->defined);
		entail = entail->next;
	}
}

struct codeln *find_codeln(int num) {
	struct item *tail = get_tail(cmdq);
	while (tail != NULL) {
		struct codeln *ln = (struct codeln *)tail->pval;
		if (ln->num == num)
			return ln;
		tail = tail->next;
	}
	return NULL;
}

void print_cmdq() {
	struct item *tail = get_tail(cmdq);
	printf("source.ps\n------------------\n");
	while (tail != NULL) {
		struct codeln *data = (struct codeln *)tail->pval;
		printf("%.4o ", data->num);
		if (data->imm)
			printf("%.6o\n", data->opcode);
		else
			printf("%.2o%.1o%.1o%.1o%.1o\n", data->opcode, data->src_t, data->src_reg, data->dest_t, data->dest_reg);
		tail = tail->next;
	}
}

void print_dataq() {
	struct item *head = get_head(cmdq);
	struct item *tail = get_tail(dataq);
	struct codeln *dt = (struct codeln *)head->pval;

	while (tail != NULL) {
		struct dataln *ln = (struct dataln *)tail->pval;
		printf("%.4o %.6o\n", ln->num + dt->num, ln->data);
		tail = tail->next;
	}
}

struct codeln *new_codeln(int opcode) {
	struct codeln *new = (struct codeln *)malloc(sizeof(struct codeln));

	new->opcode = opcode;
	new->num = ic++;

	return new;
}

struct dataln *new_dataln(int data, int num) {
	struct dataln *new = (struct dataln *)malloc(sizeof(struct dataln));

	new->data = data;
	new->num = num;

	return new;
}
