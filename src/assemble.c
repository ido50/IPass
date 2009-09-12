#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ipass.h"
#include "assembly.h"

struct error *tail = NULL;
struct error *head = NULL;

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
		int i = 0;
		while (fgets(line, MAXLINE, fp) != NULL) {
			i++;
			if (*line != ';')
				parse_line(line, i);
		}
		fclose(fp);
	}

	return tail;
}

void parse_line(char *line, int lnum) {
	unsigned short cur_state = 0;
	unsigned short line_state = 0;
	while (*line != '\0') {
		char *word = (char *)malloc(MAXLINE);
		unsigned short new_state = 0;
		if (*line == ',') {
			strncat(word, line, 1);
			line++;
		} else {
			while (isspace(*line) == 0 && *line != ',') {
				strncat(word, line, 1);
				line++;
			}
		}
		if (strlen(word) > 0) {
			new_state = parse_word(word);
			if (new_state == CMD_STATE)
				line_state = CMD_LINE;
			else if (new_state == DATA_STATE)
				line_state = INST_LINE;

			if (makes_sense(cur_state, new_state, line_state)) {
				/* this shit is ok */
				if (new_state == 1 || new_state == 2 || new_state == 5) {
					/* we need to check that labels, commands and operands are legal */
					if (legal_word(word, new_state) == 0) {
						char *msg = (char *)malloc(MAXMSG);
						strcat(msg, word);
						strcat(msg, " is not a valid ");
						strcat(msg, state_name(new_state));
						add_error(lnum, msg);
					}
				}
			} else {
				/* this shit is not ok */
				char *msg = (char *)malloc(MAXMSG);
				strcat(msg, word);
				strcat(msg, " is illegal in this context");
				add_error(lnum, msg);
			}
			cur_state = new_state;
		} else {
			line++;
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
