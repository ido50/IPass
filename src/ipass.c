#include <stdio.h>
#include <stdlib.h>
#include "ipass.h"

extern struct error *tail;

int main(int argc, char *argv[]) {

	/* check that an input file was provided. If none were provided, print usage info.
	   if more than one were provided, print an error message. */
	if (argc == 1) {
		printf("Usage: ipass [FILE]\n[FILE] is the name of an assembly source file. ipass will automatically add the .as extension to the name you provide, so don't add the extension yourself.\n");
		exit(1);
	} else if (argc > 2) {
		printf("ipass: support for more than one source file is not implemented yet.\n");
		exit(1);
	} else {
		struct error *errors = assemble(*++argv);
		if (errors != NULL) {
			/* errors were encountered, print them */
			if (errors->line == 0) {
				printf("ipass: %s\n", errors->msg);
			} else {
				printf("ipass: the following errors were encountered when assembling %s:\n", *argv);
				while (errors != NULL) {
					printf("%s[%d]: %s\n", *argv, errors->line, errors->msg);
					errors = errors->next;
				}
			}
			exit(1);
		} else {
			/* ipass executed successfully, print success message */
			printf("ipass: %s syntax OK.\n", *argv);
		}
	}		
	exit(0);
}
