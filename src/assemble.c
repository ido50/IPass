#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemble.h"

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
		
	}

	return tail;
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
