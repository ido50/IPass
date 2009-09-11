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
		struct error *new = (struct error *)malloc(sizeof(struct error));
		char *msg = (char *)malloc(MAXMSG);

		new->line = 0;
		new->next = NULL;
		strcpy(msg, "can't open ");
		strcat(msg, source);
		new->msg = msg;

		add_error(new);
	} else {
		
	}

	return tail;
}

void add_error(struct error *err) {
	if (head == 0) {
		head = err;
		tail = err;
	} else {
		head->next = err;
		head = err;
	}
}
