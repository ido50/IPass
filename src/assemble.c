#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ipass.h"
#include "assembly.h"
#include "queue.h"

static struct error *tail;
static struct error *head;

static struct queue *wordq;
static struct queue *cmdq;
static struct queue *dataq;
static struct queue *entq;
static struct queue *extq;

struct error *assemble(char *name) {
	FILE *fp;			/* source file handler */
	char *source = (char *)malloc(MAXNAME);

	/* append .as to the source file name */
	strcpy(source, name);
	strcat(source, ".as");

	/* check this source file exists */
	if ((fp = fopen(source, "r")) == NULL) {
		char *msg = (char *)malloc(MAXMSG);
		strcpy(msg, "can't open ");
		strcat(msg, source);
		add_error(0, msg);
	} else {
		char *line = (char *)malloc(MAXLINE);
		struct item *tail = (struct item *)malloc(sizeof(struct item));
		int i = 0;

		wordq = new_queue();
		cmdq = new_queue();
		dataq = new_queue();
		entq = new_queue();
		extq = new_queue();

		while (fgets(line, MAXLINE, fp) != NULL) {
			i++;
			if (*line != ';')
				break_line(line, i);
		}
		fclose(fp);

		tail = get_tail(dataq);
		while (tail != 0) {
			char *data = (char *)tail->data;
			if (strlen(data) > 1) {
				while (*data != '\0') {
					printf("%.6o\n", *data);
					data++;
				}
				printf("%.6o\n", 0);
			} else
				printf("%.6o\n", atoi(data));
			tail = tail->next;
		}
	}

	return tail;
}

void break_line(char *line, int lnum) {
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
	parse_line(lnum);
}

void parse_line(int lnum) {
	struct item *it = shift(wordq);
	char *word = (char *)it->data;
	char *label = (char *)malloc(MAXLABEL);

	if (is_label(word)) {
		label = label_name(word);
		if (legal_label(label) == 0) {
			char *msg = (char *)malloc(MAXMSG);
			strcat(msg, label);
			strcat(msg, " is not a valid label.");
			add_error(lnum, msg);
			label = NULL;
		}
	} else
		unshift(wordq, word);

	if (is_empty(wordq))
		add_error(lnum, "void line");
	else {
		struct item *next = shift(wordq);
		word = (char *)next->data;
		if (is_cmd(word)) {
			/* check cmd name is legal */
			if (legal_cmd(word) == 0) {
				char *msg = (char *)malloc(MAXMSG);
				strcat(msg, word);
				strcat(msg, " is not a valid command.");
				add_error(lnum, msg);
			} else {
				/* get operands and check that they are legal */
				/* do something with the label */
				int expect_comma = 0;
				struct command *cmd = find_cmd(word);

				if (cmd->src_op && cmd->dest_op)
					expect_comma = 1;

				if (cmd->src_op) {
					/* get source operand */
					struct item *source = shift(wordq);
					if (source == NULL)
						/* there is no source, which means there is no destination too */
						add_error(lnum, "source and destination operands missing");
					else {
						/* do something with the source */
						char *src_n = (char *)source->data;
						unsigned short src_t = op_type(src_n);
					}
				}
				if (cmd->dest_op) {
					struct item *dest = (struct item *)malloc(sizeof(struct item));

					if (expect_comma) {
						/* get comma */
						struct item *comma = shift(wordq);
						if (comma == NULL || ! is_comma((char *)comma->data))
							add_error(lnum, "missing comma and/or destination operand");
					}

					/* get destination operand */
					if ((dest = shift(wordq)) == NULL)
						add_error(lnum, "destination operand missing");
					else {
						/* do smoething with the destination */
						char *dest_n = (char *)dest->data;
						unsigned short dest_t = op_type(dest_n);
					}
				}
			}
		} else if (is_data_inst(word)) {
			/* get data and check that it's legal */
			int expect_num = 1;
			int expect_comma = 0;
			if (is_empty(wordq))
				add_error(lnum, "no data given");
			else {
				while (! is_empty(wordq)) {
					struct item *next = shift(wordq);
					char *data = (char *)next->data;
					if (expect_num && is_num(data) == 0) {
						char *msg = (char *)malloc(MAXMSG);
						strcat(msg, data);
						strcat(msg, " is not a valid number.");
						add_error(lnum, msg);
					} else if (expect_num) {
						/* do something with this number */
						push(dataq, data);
						expect_comma = 1;
						expect_num = 0;
					} else if (expect_comma && is_comma(data) == 0) {
						/* check if there are no more words */
						if (! is_empty(wordq)) {
							add_error(lnum, "i was expecting a comma or end of line");
							expect_num = 0;
							expect_comma = 0;
						}
					} else if (expect_comma) {
						expect_num = 1;
						expect_comma = 0;
						if (is_empty(wordq))
							add_error(lnum, "no number given after comma");
					}
				}
			}
		} else if (is_string_inst(word)) {
			/* get string and check that it's legal */
			struct item *next = shift(wordq);
			if (next == NULL)
				add_error(lnum, "no string given");
			else {
				char *string = (char *)next->data;
				if (is_string(string)) {
					/* remove the '"' signs from the string */
					char *str = (char *)malloc(MAXLINE);
					string++;
					strncpy(str, string, strlen(string)-1);
					/* code that string */
					push(dataq, str);
				} else {
					/* add an error */
					char *msg = (char *)malloc(MAXMSG);
					strcat(msg, string);
					strcat(msg, " is not a valid string");
					add_error(lnum, msg);
				}
			}
		} else if (is_entry(word) || is_extern(word)) {
			/* get a label and check it exists */
			struct item *next = shift(wordq);
			if (next == NULL)
				add_error(lnum, "missing label name");
			else {
				char *label = (char *)next->data;
				if (legal_label(label) == 0) {
					char *msg = (char *)malloc(MAXMSG);
					strcat(msg, label);
					strcat(msg, " is not a valid label");
					add_error(lnum, msg);
				} else {
					/* do something with that label */
				}
				if (! is_empty(wordq))
					add_error(lnum, "invalid number of labels");
			}
		}
	}

}

void add_error(int line, char *msg) {
	struct error *err = (struct error *)malloc(sizeof(struct error));
	err->line = line;
	err->msg = msg;
	err->next = NULL;

	if (head == 0) {
		head = err;
		tail = err;
	} else {
		head->next = err;
		head = err;
	}
}

void print_queue(struct queue *q) {
	struct item *tail = get_tail(q);
	while (tail != 0) {
		printf("%s ", (char *)tail->data);
		tail = tail->next;
	}
	printf("\n");
}
