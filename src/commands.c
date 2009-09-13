#include <stdlib.h>
#include <string.h>
#include "assembly.h"
#include "queue.h"
#include "lines.h"

struct command commands[NUMCMDS] = {
	{ "mov", 0, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "cmp", 1, 1,1, 1,1,1,1, 1,1,1,1 },
	{ "add", 2, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "sub", 3, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "and", 4, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "xor", 5, 1,1, 1,1,1,1, 0,1,1,1 },
	{ "lea", 6, 1,1, 0,1,0,0, 0,1,1,1 },
	{ "inc", 7, 0,1, 0,0,0,0, 0,1,1,1 },
	{ "dec", 8, 0,1, 0,0,0,0, 0,1,1,1 },
	{ "jmp", 9, 0,1, 0,0,0,0, 0,1,1,0 },
	{ "bne", 10, 0,1, 0,0,0,0, 0,1,1,0 },
	{ "red", 11, 0,1, 0,0,0,0, 0,1,1,1 },
	{ "prn", 12, 0,1, 0,0,0,0, 1,1,1,1 },
	{ "jsr", 13, 0,1, 0,0,0,0, 0,1,1,0 },
	{ "rts", 14, 0,0, 0,0,0,0, 0,0,0,0 },
	{ "hlt", 15, 0,0, 0,0,0,0, 0,0,0,0 },
};

struct command *find_cmd(char *name) {
	int i;

	for (i = 0; i < NUMCMDS; i++)
		if (strcmp(name, commands[i].name) == 0)
			return &commands[i];
	return NULL;
}
