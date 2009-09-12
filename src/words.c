#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "assembly.h"

int is_num(char *word) {
	if (*word == '+' || *word == '-')
		word++;

	while (*word != 0) {
		if (isdigit(*word) == 0)
			return 0;
		word++;
	}

	return 1;
}

int is_label(char *word) {
	if (word[strlen(word)-1] == ':')
		return 1;
	return 0;
}

int legal_label(char *word) {
	if (strlen(word) > (MAXLABEL + 1))
		/* including the ':' sign */
		return 0;
	if (isalpha(*word) == 0)
		return 0;
	word++;
	while (*++word != ':') {
		if (isalnum(*word) == 0)
			return 0;
	}

	return 1;
}

char *label_name(char *word) {
	if (is_label(word)) {
		char *name = (char *)malloc(MAXLABEL);
		strncpy(name, word, strlen(word)-1);
		return name;
	}
	return NULL;
}

int is_string(char *word) {
	if (word[0] == '"' && word[strlen(word)-1] == '"')
		return 1;
	return 0;
}

int is_cmd(char *word) {
	if (strlen(word) == 3 &&
	    word[0] >= 'a' && word[0] <= 'z' &&
	    word[1] >= 'a' && word[1] <= 'z' &&
	    word[2] >= 'a' && word[2] <= 'z')
		return 1;

	return 0;
}

int legal_cmd(char *word) {
	if (find_cmd(word) != NULL)
		return 1;
	return 0;
}

int is_data_inst(char *word) {
	if (strcmp(word, ".data") == 0)
		return 1;
	return 0;
}

int is_string_inst(char *word) {
	if (strcmp(word, ".string") == 0)
		return 1;
	return 0;
}

int is_comma(char *word) {
	if (strcmp(word, ",") == 0)
		return 1;
	return 0;
}

int parse_word(char *word) {
	if (is_label(word))
		return 1;
	else if (is_cmd(word))
		return 2;
	else if (is_data_inst(word))
		return 3;
	else if (is_string_inst(word))
		return 4;
	else if (is_num(word))
		return 6;
	else if (is_string(word))
		return 7;
	else if (is_comma(word))
		return 8;
	return 5;
}
