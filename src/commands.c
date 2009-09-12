#include <stdlib.h>
#include <string.h>
#include "assembly.h"

struct command commands[NUMCMDS] = {
	{ "mov", 00, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "cmp", 01, 1,1, 1,1,1,1, 1,1,1,1 },
	{ "add", 02, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "sub", 03, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "and", 04, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "xor", 05, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "lea", 06, 1,1, 0,1,0,0, 0,1,1,1 },
	{ "inc", 07, 0,1, 0,0,0,0, 0,1,1,1 },
	{ "dec", 10, 0,1, 0,0,0,0, 0,1,1,1 },
	{ "jmp", 11, 0,1, 0,0,0,0, 0,1,1,0 },
	{ "bne", 12, 0,1, 0,0,0,0, 0,1,1,0 },
	{ "red", 13, 0,1, 0,0,0,0, 0,1,1,1 },
	{ "prn", 14, 0,1, 0,0,0,0, 1,1,1,1 },
	{ "jsr", 15, 0,1, 0,0,0,0, 0,1,1,0 },
	{ "rts", 16, 0,0, 0,0,0,0, 0,0,0,0 },
	{ "hlt", 17, 0,0, 0,0,0,0, 0,0,0,0 },
};

struct command *find_cmd(char *name) {
	int i;

	for (i = 0; i < NUMCMDS; i++)
		if (strcmp(name, commands[i].name) == 0)
			return &commands[i];
	return NULL;
}
