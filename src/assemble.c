#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "assembly.h"
#include "queue.h"
#include "symbols.h"
#include "lines.h"

static struct queue *wordq;		/* the queue of words extracted from the source file */
static struct queue *cmdq;		/* the queue of command lines to be printed to the output object file */
static struct queue *dataq;		/* the queue of data lines to be printed to the output object file */
static struct queue *entq;		/* the queue of entry symbols to be printed to the entry file */
static struct queue *extq;		/* the queue of external symbols to be printed to the extern file */
static struct queue *symt;		/* the symbol table, which is just a queue, built during assembling the source code */
static struct queue *errors;		/* the queue of errors encountered during the assembly */

static unsigned ic, dc, lnum;		/* the instruction counter, data counter and current line number */

/**
 * Assembles an assembly file represented by a name
 * 
 * @param name - the name of the file (without the ".as" extension)
 * @return	pointer to the errors queue (even if no errors where encountered)
 */
struct queue *assemble(char *name) {
	FILE *fp;					/* source file handler */
	char *source = (char *)malloc(MAXNAME);

	/* append .as to the source file name */
	strcpy(source, name);
	strcat(source, ".as");

	/* remove queues left off from previous source files */
	del_queue(wordq);
	del_queue(cmdq);
	del_queue(dataq);
	del_queue(entq);
	del_queue(extq);
	del_queue(symt);
	del_queue(errors);

	/* initialize the queues again */
	wordq = new_queue();
	cmdq = new_queue();
	dataq = new_queue();
	entq = new_queue();
	extq = new_queue();
	symt = new_queue();
	errors = new_queue();

	/* initalize the counters */
	ic = 0;
	dc = 0;
	lnum = 0;

	/* check if source file exists */
	if ((fp = fopen(source, "r")) == NULL)
		/* source file doesn't exist */
		_add_error("can't open ", source, " for reading", "end");
	else {
		/* read the file line by line */
		char *line = (char *)malloc(MAXLINE);
		while (fgets(line, MAXLINE, fp) != NULL) {
			lnum++; /* increase line number counter */
			if (*line != ';')
				/* break line into words and parse it */
				_break_line(line);
		}
		/* close the file */
		fclose(fp);

		/* update all the symbols in the table */
		_update_syms();

		/* unless errors were encountered, print the assembled output files */
		if (is_empty(errors)) {
			_print_syms(name);
			_print_ob(name);
		}

		/* free memory used by the line variable */
		free(line);
	}

	/* free memory used by the source variable */
	free(source);

	/* return the error queue */
	return errors;
}

void _break_line(char *line) {
	/* breaks the line into individual words */

	/* clear the word queue left from the previous line */
	clear_queue(wordq);

	/* traverse the line char by char */
	while (*line != '\0') {
		char *word = (char *)malloc(MAXLINE);
		if (*line == ',') {
			/* treat a comma is a single word */
			strncat(word, line, 1);
			line++;
		} else {
			while (isspace(*line) == 0 && *line != ',') {
				/* character is inside a word, concatenate it with the current parsed word */
				strncat(word, line, 1);
				line++;
			}
		}
		if (strlen(word) > 0)
			/* we have a word, push it to the word queue */
			push(wordq, word);
		else
			line++;
	}

	/* parse the words of this line */
	_parse_words();
}

void _parse_words() {
	char *label = _get_label(lnum);

	if (is_empty(wordq))
		_add_error("void line", "end");
	else {
		struct item *next = shift(wordq);
		char *word = (char *)next->data;
		if (is_cmd(word)) {
			/* check cmd name is legal */
			if (legal_cmd(word) == 0)
				_add_error(word, " is not a valid command", "end");
			else {
				/* get operands and check that they are legal */
				/* do something with the label */
				int expect_comma = 0;
				struct command *cmd = find_cmd(word);
				struct codeln *cmdln = _new_codeln(cmd->code);
				cmdln->type = 'a'; /* useless as it's the default */

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
				_add_error("no data given", "end");
			else {
				int expect_num = 1;
				int expect_comma = 0;

				while (! is_empty(wordq)) {
					struct item *next = shift(wordq);
					char *data = (char *)next->data;

					if (expect_num && ! is_num(data))
						_add_error(data, " is not a valid number", "end");
					else if (expect_num) {
						i++;
						/* do something with this number */
						push(dataq, _new_dataln(atoi(data), dc + i));
						expect_comma = 1;
						expect_num = 0;
					} else if (expect_comma && ! is_comma(data)) {
						/* check if there are no more words */
						if (! is_empty(wordq)) {
							_add_error("i was expecting a comma or end of line", "end");
							expect_num = 0;
							expect_comma = 0;
						}
					} else if (expect_comma) {
						expect_num = 1;
						expect_comma = 0;
						if (is_empty(wordq))
							_add_error("no number given after comma", "end");
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
				_add_error("no string given", "end");
			else {
				char *string = (char *)next->data;
				if (is_string(string)) {
					/* remove the '"' signs from the string */
					char *str = (char *)malloc(MAXLINE);

					string++;
					strncpy(str, string, strlen(string)-1);
					/* code that string */
					while (*str != '\0') {
						i++;
						push(dataq, _new_dataln(*str, dc+i));
						str++;
					}
					push(dataq, _new_dataln(0, dc + ++i));
				} else
					_add_error(string, " is not a valid string", "end");
			}

			if (i > 0)
				_treat_label(label, dc + 1, 1);
			dc += i;
		} else if (is_entry(word) || is_extern(word)) {
			/* get a label and check it exists */
			struct item *lbl = shift(wordq);
			if (lbl == NULL)
				_add_error("missing label name", "end");
			else {
				char *label = (char *)lbl->data;
				if (is_entry(word))
					push(entq, label);
				else
					push(extq, label);
			}
		} else
			_add_error("unrecognized word \"", word, "\"", "end");
	}
}

char *_get_label() {
	/* shifts a word from the word queue expecting a label.
	   if word is not a label, unshifts the word back to the queue */

	struct item *it = shift(wordq);
	char *word = (char *)it->data;
	char *label = (char *)malloc(MAXLABEL);

	if (is_label(word)) {
		label = label_name(word);
		if (legal_label(label) == 0) {
			/* this is not a legal label */
			_add_error(label, " is not a valid label.", "end");
			label = NULL;
		}
	} else {
		/* return NULL and unshift word back to the queue */
		label = NULL;
		unshift(wordq, word);
	}
	return label;
}

void _get_operand(struct codeln *cmdln, struct command *cmd, char *srcdst, int expect_comma) {
	if (is_empty(wordq))
		/* no operand */
		_add_error(srcdst, " operand is missing", "end");
	else {
		if (expect_comma) {
			struct item *opword = shift(wordq);
			if (! is_comma((char *)opword->data)) {
				unshift(wordq, opword);
				_add_error("missing comma between operands", "end");
			}
		}

		if (is_empty(wordq))
			_add_error(srcdst, " operand is missing", "end");
		else {
			int type;
			struct item *opword = shift(wordq);
			char *name = (char *)opword->data;
			type = op_type(name);

			_add_type(cmdln, srcdst, type);

			if (! _allowed(cmd, srcdst, type))
				_add_error(cmd->name, " does not allow ", _address_name(type), " addressing", "end");

			if (type == DIR || type == INDIR) {
				struct codeln *opln = _new_codeln(0);
				struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
				opln->imm = 1;
				opln->type = 'r';

				if (type == INDIR)
					/* increase to get rid of the @ sign */
					name++;

				if ((sym = find_symbol(symt, name)) != NULL)
					update_used(sym, opln->num);
				else
					push(symt, new_symbol(name, -1, opln->num, 0));
				push(cmdq, opln);
			} else if (type == IMM) {
				struct codeln *opln = _new_codeln(0);
				opln->imm = 1;
				opln->type = 'a'; /* useless as it's the default */

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

void _add_type(struct codeln *cmdln, char *srcdst, int type) {
	/* adds the addressing method of the source or destination of a command line,
	   i.e. immediate, direct, indirect or direct register */

	if (strcmp(srcdst, "src") == 0)
		cmdln->src_t = type;
	else
		cmdln->dest_t = type;
}

int _allowed(struct command *cmd, char *srcdst, int type) {
	/* checks if a specific addressing method is allowed by a command */

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
	/* adds the number of a register to the source or destination of a command line */

	if (strcmp(srcdst, "src") == 0)
		cmdln->src_reg = name;
	else
		cmdln->dest_reg = name;
}

char *_address_name(int type) {
	/* returns the name of an addressing method, used for printing useful errors */

	if (type == IMM)
		return "immediate";
	else if (type == DIR)
		return "direct";
	else if (type == INDIR)
		return "indirect";
	else
		return "direct register";
}

void _treat_label(char *label, unsigned num, int type) {
	/* takes a label that was defined in a code line and adds it to the symbol table.
	   it it's already there (because was used in a previous line) just updates the its
	   defined value, otherwise adds it to the table. If it was already defined, adds an error. */

	if (label != NULL) {
		struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
		if ((sym = find_symbol(symt, label)) != NULL) {
			if (sym->defined >= 0)
				/* this symbol was already defined */
				_add_error(label, " was already defined", "end");
			else
				update_defined(sym, num, type);
		} else
			push(symt, new_symbol(label, num, -1, type));
	}
}

void _update_syms() {
	/* updates the symbols table with information gathered while
	   completing the traversal of the source file */

	struct item *tail = get_tail(symt);

	/* update external symbols (if any) */
	if (! is_empty(extq)) {
		/* there were extern declarations in the source file */
		struct item *extail = get_tail(extq);

		/* set the defined value of external labels to zero */
		while (extail != NULL) {
			char *label = (char *)extail->data;
			struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
			if ((sym = find_symbol(symt, label)) != NULL) {
				unsigned i;
				for (i = 0; i < sym->num_used; i++)
					sym->defined = 0;
			}
			extail = extail->next;
		}
	}

	/* update all symbols */
	while (tail != NULL) {
		struct symbol *sym = (struct symbol *)tail->data;
		if (sym->type == 1)
			/* it's a data symbol, increase its defined value with ic -1 */
			sym->defined += ic - 1;
		if (sym->num_used > 0) {
			unsigned i;
			/* go back to the lines where this symbol was used and update their
			   code with the number of the line where this symbol was defined. */
			for (i = 0; i < sym->num_used; i++) {
				struct codeln *line = _find_codeln(sym->used[i]);
				if (line != NULL) {
					if (sym->defined == 0)
						/* this is an external line */
						line->type = 'e';
					/* update the code */
					line->opcode = sym->defined;
				}
			}
		}
		if (sym->defined < 0)
			/* this symbol was not defined anywhere in the source file, add an error */
			_add_error(sym->name, " was not defined anywhere", "end");
		tail = tail->next;
	}

	/* update symbols that had an entry declaration */
	if (! is_empty(entq)) {
		struct item *entail = get_tail(entq);

		while (entail != NULL) {
			char *label = (char *)entail->data;
			struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
			if ((sym = find_symbol(symt, label)) != NULL)
				/* set this symbol as an entry symbol */
				sym->entry = 1;
			entail = entail->next;
		}
	}
}

void _print_syms(char *name) {
	/* print all entry and extern symbols into the output .ent and .ext files */

	if (! is_empty(extq)) {
		/* there are external symbols */
		FILE *of;
		char *fname = (char *)malloc(MAXNAME);
		strcpy(fname, name);
		strcat(fname, ".ext");

		if ((of = fopen(fname, "w")) == NULL)
			_add_error("can't open ", fname, "for writing", "end");
		else {
			unsigned i;
			for (i = 0; i <= ic; i++) {
				struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
				if ((sym = find_symbol_used_in(symt, i)) != NULL && sym->defined == 0)
					/* print the symbol's name and the line where it is used */
					fprintf(of, "%s %o\n", sym->name, i);
			}
		}
	}

	if (! is_empty(entq)) {
		/* there are entry symbols */

		FILE *of;
		char *fname = (char *)malloc(MAXNAME);
		strcpy(fname, name);
		strcat(fname, ".ent");

		if ((of = fopen(fname, "w")) == NULL)
			_add_error("can't open ", fname, "for writing", "end");
		else {
			unsigned i;
			for (i = 0; i <= ic + dc; i++) {
				struct symbol *sym = (struct symbol *)malloc(sizeof(struct symbol));
				if ((sym = find_symbol_defined_in(symt, i)) != NULL && sym->entry)
					/* print the symbol's name and the line where it is defined */
					fprintf(of, "%s %o\n", sym->name, sym->defined);
			}
		}
	}
}

void _print_ob(char *name) {
	/* print the command and data queues to the output ".ob" file */
	FILE *of;
	struct item *cmdtail = get_tail(cmdq);			/* tail of the command queue */
	struct item *cmdhead = get_head(cmdq);			/* head of the command queue */
	struct item *datatail = get_tail(dataq);		/* tail of the data queue */
	struct codeln *dt = (struct codeln *)cmdhead->data;	/* the command queue's head's data */
	char *fname = (char *)malloc(MAXNAME);

	strcpy(fname, name);
	strcat(fname, ".ob"); /* output file name */

	if ((of = fopen(fname, "w")) == NULL)
		_add_error("can't open ", fname, " for writing", "end");
	else {
		fprintf(of, "%o %o\n", ic, dc);
		while (cmdtail != NULL) {
			struct codeln *data = (struct codeln *)cmdtail->data;
			fprintf(of, "%.4o ", data->num);
			if (data->imm)
				/* this is an immediate value, just print its opcode */
				fprintf(of, "%.6o %c\n", data->opcode, data->type);
			else
				/* this is a regular command line, print it */
				fprintf(of, "%.2o%.1o%.1o%.1o%.1o %c\n", data->opcode, data->src_t, data->src_reg, data->dest_t, data->dest_reg, data->type);
			cmdtail = cmdtail->next;
		}

		while (datatail != NULL) {
			struct dataln *ln = (struct dataln *)datatail->data;
			/* print the data line */
			fprintf(of, "%.4o %.6o\n", ln->num + dt->num, ln->data);
			datatail = datatail->next;
		}
	}
}

struct codeln *_find_codeln(int num) {
	/* finds the code line with the provided number */

	struct item *tail = get_tail(cmdq);
	while (tail != NULL) {
		struct codeln *ln = (struct codeln *)tail->data;
		if (ln->num == num)
			return ln;
		tail = tail->next;
	}
	return NULL;
}

struct codeln *_new_codeln(int opcode) {
	/* creates a new code line with the provided (maybe temporary) opcode,
	   without pushing it to the command queue */

	struct codeln *new = (struct codeln *)malloc(sizeof(struct codeln));

	new->opcode = opcode;
	new->num = ic++;
	new->type = 'a'; /* automatically set to absolute line, will be changes if needed */

	return new;
}

struct dataln *_new_dataln(int data, int num) {
	/* creates a new data line with the provided data and line number,
	   without pushing it to the data queue */

	struct dataln *new = (struct dataln *)malloc(sizeof(struct dataln));

	new->data = data;
	new->num = num;

	return new;
}

void _add_error(char *msg1, ...) {
	/* adds a new error to the errors queue */

	char *i = (char *)malloc(MAXMSG);
	char *msg = (char *)malloc(MAXMSG);
	struct error *err = (struct error *)malloc(sizeof(struct error));

	/* traverse the argument list */
	va_list ap;

	err->line = lnum;

	/* initialize ptr to point to the first argument after the format string */
	va_start(ap, msg1);

	for (i = msg1; strcmp(i, "end") != 0; i = va_arg(ap, char *))
		strcat(msg, i);
	va_end(ap);

	err->msg = msg;

	/* push the error to the queue */
	push(errors, err);
}
