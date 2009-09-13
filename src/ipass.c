#include <stdio.h>
#include <stdlib.h>
#include "assembly.h"
#include "queue.h"

int main(int argc, char *argv[]) {
	if (argc == 1) {
		/* no source files were provied, print usage information */

		/* check if the provided README file is still here */
		FILE *fp = fopen("README", "r");
		if (fp == NULL)
			/* print a basic usage line */
			printf("ipass - Ido Perlmuter's Assembler\nusage: ipass [FILE] ...\n\nipass will try to assemble the FILEs. If errors are encountered, they will be\nprinted to the standard output, otherwise octal output files will be created.\nFile names must have an extension \".as\", ipass will automatically add it to\nthe names you provide, so don't add the extension yourself.\n");
		else {
			/* print the pretty man page */
			char *line = (char *)malloc(MAXLINE);
			while (fgets(line, MAXLINE, fp) != NULL)
				printf("%s", line);
			free(line);
		}
		exit(1);
	} else {
		int i;
		/* traverse the arguments */
		for (i = 1; i < argc; i++) {
			/* attempt to assemble the file */
			struct queue *errors = assemble(argv[i]);

			/* check if errors were found, and if so, print them */
			if (! is_empty(errors)) {
				struct item *tail = get_tail(errors);
				printf("%s.as: the following errors were encountered:\n", argv[i]);
				while (tail != NULL) {
					struct error *err = (struct error *)tail->data;
					/* errors were encountered, print them */
					printf("%s.as:[%d] %s\n", argv[i], err->line, err->msg);
					tail = tail->next;
				}
			} else {
				/* file was assembled successfully, print success message */
				printf("%s.as: successfully assembled\n", argv[i]);
			}
			printf("\n");
		}
	}		
	exit(0);
}
