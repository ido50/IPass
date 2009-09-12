#include "assembly.h"

char *state_names[9] = { "new line", "label", "command", "data instruction", "string instruction", "operand", "number", "string", "comma" };

unsigned short states[9][9] = {
	{0, 1, 1, 1, 1, 0, 0, 0, 0},
	{0, 0, 1, 1, 1, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 1, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 1, 0, 0},
};

int makes_sense(unsigned short cur_state, unsigned short new_state, unsigned short line_state) {
	if (line_state == CMD_LINE) {
		states[8][5] = 1;
		states[8][6] = 0;
	} else if (line_state == INST_LINE) {
		states[8][5] = 0;
		states[8][6] = 1;
	} else {
		states[8][5] = 0;
		states[8][6] = 0;
	}

	if (states[cur_state][new_state] == 1)
		return 1;
	else
		return 0;
}

char *state_name(unsigned short state) {
	return state_names[state];
}
