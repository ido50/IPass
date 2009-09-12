#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ipass.h"
#include "assembly.h"
#include "queue.h"

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
				break_line(line, i);
		}
		fclose(fp);
	}

	return tail;
}

void break_line(char *line, int lnum) {
	clear_queue();
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
			unshift(word);
		else
			line++;
	}
	parse_line(lnum);
}

void parse_line(int lnum) {
	struct item *tail = shift();
	char *word = (char *)tail->data;
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
	}

	tail = shift();
	word = (char *)tail->data;
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
		}
	} else if (is_data_inst(word)) {
		/* get data and check that it's legal */
		int expect_num = 1;
		while (expect_num) {
			struct item *it = shift();
			char *data = (char *)it->data;
			
			if (is_num(data) == 0) {
				char *msg = (char *)malloc(MAXMSG);
				strcat(msg, data);
				strcat(msg, " is not a valid number.");
				add_error(lnum, msg);
			} else {
			}
			it = shift();
			data = (char *)it->data;
			if (data == NULL)
				expect_num = 0;
			else if (is_comma(data))
				expect_num = 1;
			else {
				char *msg = (char *)malloc(MAXMSG);
				strcat(msg, "can't understand ");
				strcat(msg, data);
				strcat(msg, ", was expecting a comma or new line");
				add_error(lnum, msg);
			}
		}
	} else if (is_string_inst(word)) {
		/* get string and check that it's legal */
		struct item *it = shift();
		char *string = (char *)it->data;
		if (is_string(string)) {
			/* remove the '"' signs from the string */
			char *str = (char *)malloc(MAXLINE);
			string++;
			strncpy(str, string, strlen(string)-1);
			/* code that string */
		} else {
			/* add an error */
			char *msg = (char *)malloc(MAXMSG);
			strcat(msg, string);
			strcat(msg, " is not a valid string");
			add_error(lnum, msg);
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
