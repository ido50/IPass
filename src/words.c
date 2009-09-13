#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "assembly.h"

/**
 * Returns the addressing method of an operand according to its name
 * 
 * @param name - the name of the operand
 * @return	an integer denoting the operand's addressing method
 */
int op_type(char *name) {
	if (is_inum(name))
		/* this is a number of the sort #5 */
		return IMM;
	else if (is_register(name))
		/* this is the name of a register like r7 */
		return DIRREG;
	else if (is_ilabel(name))
		/* this is an indirect symbol like @LABEL */
		return INDIR;
	else if (legal_label(name))
		/* this is a direct symbol like LABEL */
		return DIR;
	/* I don't know what this operand is */
	return -1;
}

/**
 * Checks if a word (i.e. character string) represents a number
 * 
 * @param word - the word to be checked
 * @return	true value if word is a number, zero otherwise
 */
int is_num(char *word) {
	if (*word == '+' || *word == '-')
		/* first char is a sign, continue to the second char */
		word++;

	while (*word != 0) {
		if (! isdigit(*word))
			/* this char is not a digit, return zero */
			return 0;
		word++;
	}

	return 1;
}

/**
 * Checks if a word (i.e. character string) represents a "literal",
 * or "immediate" number, in a source file (like #-5).
 * 
 * @param word - the word to be checked
 * @return	true value if word is a number, zero otherwise
 */
int is_inum(char *word) {
	/* all we need to check is that the first character is '#'
	   and that the rest represent a number */
	if (word[0] == '#' && is_num(++word))
		return 1;
	return 0;
}

/**
 * Checks if a word in a source file represents a label. This function
 * does NOT check if the word is valid label according to the language's rules.
 * 
 * @param word - the word to be checked
 * @return	true value if word is a label, zero otherwise
 */
int is_label(char *word) {
	/* check that the last character is ':' */
	if (word[strlen(word)-1] == ':')
		return 1;
	return 0;
}

/**
 * Checks if a word is valid according to the language's rules.
 * 
 * @param word - the word to be checked
 * @return	true value if word is a valid label, zero otherwise
 */
int legal_label(char *word) {
	if (strlen(word) > MAXLABEL)
		/* too many characters */
		return 0;
	if (isalpha(*word) == 0)
		/* first character is not alphabetic */
		return 0;
	while (*++word != 0)
		if (isalnum(*word) == 0)
			/* this character is not alphanumeric */
			return 0;

	return 1;
}

/**
 * Checks if a word denotes an indirect symbol, like @LABEL. This function
 * DOES check if the label is valid according to the language's rules.
 * 
 * @param word - the word to be checked
 * @return	true value if word is an indirect label, zero otherwise
 */
int is_ilabel(char *word) {
	if (word[0] == '@' && legal_label(++word))
		return 1;
	return 0;
}

/**
 * Returns the name of a label from a word that defines it, i.e. removes
 * the trailing ':'. If the word doesn't have the trailing ':' (i.e. it
 * is not a label), then NULL is returned.
 * 
 * @param word - the word
 * @return	character string of the label's name
 */
char *label_name(char *word) {
	if (is_label(word)) {
		char *name = (char *)malloc(MAXLABEL);
		strncpy(name, word, strlen(word)-1);
		return name;
	}
	return NULL;
}

/**
 * Checks if a word is a literal string inside a source file
 * 
 * @param word - the word to be checked
 * @return	true value if word is a string, zero otherwise
 */
int is_string(char *word) {
	if (word[0] == '"' && word[strlen(word)-1] == '"')
		return 1;
	return 0;
}

/**
 * Checks if a word might be a command name. This function does not
 * check if such a command actually exists.
 * 
 * @param word - the word to be checked
 * @return	true value if word might be a command, zero otherwise
 */
int is_cmd(char *word) {
	if (strlen(word) == 3 && isalpha(word[0]) && isalpha(word[1]) && isalpha(word[2]))
		/* the word's length is three and consists of alphabetic characters only,
		   so it might be a command name */
		return 1;

	return 0;
}

/**
 * Checks if a word might represents a valid command name.
 * 
 * @param word - the word to be checked
 * @return	true value if word is a valid command, zero otherwise
 */
int legal_cmd(char *word) {
	if (find_cmd(word) != NULL)
		/* find_cmd is declared in commands.h */
		return 1;
	return 0;
}

/**
 * Checks if a word is a data instruction, i.e. if it is ".data".
 * 
 * @param word - the word to be checked
 * @return	true value if word is a data instruction, zero otherwise
 */
int is_data_inst(char *word) {
	if (strcmp(word, ".data") == 0)
		return 1;
	return 0;
}

/**
 * Checks if a word is a string instruction, i.e. if it is ".string".
 * 
 * @param word - the word to be checked
 * @return	true value if word is a string instruction, zero otherwise
 */
int is_string_inst(char *word) {
	if (strcmp(word, ".string") == 0)
		return 1;
	return 0;
}

/**
 * Checks if a word is actually just a comma.
 * 
 * @param word - the word to be checked
 * @return	true value if word is a comma, zero otherwise
 */
int is_comma(char *word) {
	if (strcmp(word, ",") == 0)
		return 1;
	return 0;
}

/**
 * Checks if a word is an entry instruction, i.e. if it is ".entry"
 * 
 * @param word - the word to be checked
 * @return	true value if word is an entry instruction, zero otherwise
 */
int is_entry(char *word) {
	if (strcmp(word, ".entry") == 0)
		return 1;
	return 0;
}

/**
 * Checks if a word is an extern instruction, i.e. if it is ".extern"
 * 
 * @param word - the word to be checked
 * @return	true value if word is an extern instruction, zero otherwise
 */
int is_extern(char *word) {
	if (strcmp(word, ".extern") == 0)
		return 1;
	return 0;
}

/**
 * Checks if a word is the name of a register like "r5". This function does
 * not check if such a register actually exists.
 * 
 * @param word - the word to be checked
 * @return	true value if word is a register name, zero otherwise
 */
int is_register(char *word) {
	if (strlen(word) == 2 && word[0] == 'r' && isdigit(word[1]))
		return 1;
	return 0;
}

/**
 * Checks if a word represents a valid register name (i.e. one of r0 up
 * to r7).
 * 
 * @param word - the word to be checked
 * @return	true value if word is a valid register, zero otherwise
 */
int legal_register(char *word) {
	if (is_register(word))
		if (word[1] >= 0 && word[1] <= 7)
			return 1;
	return 0;
}
