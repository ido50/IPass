#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "symbols.h"

/**
 * Creates a new symbol
 * 
 * @param name - name of the symbol (i.e. the label)
 * @param defined - the number of the code line where it was defined (use -1 if unknown yet)
 * @param used - the number of the code line where it was first used (use -1 if not yet used)
 * @param type - the type of the symbol (0 for instruction symbol, 1 for data symbol)
 * @return	pointer to the new symbol
 */
struct symbol *new_symbol(char *name, int defined, unsigned used, int type) {
	struct symbol *new = (struct symbol *)malloc(sizeof(struct symbol));

	new->name = name;
	new->defined = defined;
	new->type = type;
	new->used[new->num_used++] = used;
	new->entry = 0;

	return new;
}

/**
 * Finds a symbol in a symbol table according to name
 * 
 * @param table - pointer to the symbol table (i.e. queue)
 * @param name - name of the symbol (i.e. the label)
 * @return	pointer to the found symbol or NULL if not found
 */
struct symbol *find_symbol(struct queue *table, char *name) {
	/* traverse the symbol table */
	struct item *it = table->tail;
	while (it != NULL) {
		struct symbol *sym = (struct symbol *)it->data;
		if (strcmp(sym->name, name) == 0)
			/* we've found it */
			return sym;
		it = it->next;
	}
	/* symbol was not found, return NULL */
	return NULL;
}

/**
 * Adds the number of a code line where a symbol was used
 * 
 * @param sym - pointer to the symbol that was used
 * @param used - the number of the code line where it was used
 */
void update_used(struct symbol *sym, unsigned used) {
	sym->used[sym->num_used++] = used;
}

/**
 * Updates the number of the code line where a symbol was defined
 * and the type of the symbol.
 * 
 * @param sym - pointer to the symbol that was defined
 * @param defined - the number of the code line where it was defined
 * @param type - the type of the symbol (0 for instruction symbol, 1 for data symbol)
 */
void update_defined(struct symbol *sym, int defined, int type) {
	sym->defined = defined;
	sym->type = type;
}

/**
 * Finds the symbol that is referred to in a code line
 * 
 * @param table - pointer to the symbol table (i.e. queue)
 * @param used - the number of the code line
 * @return	pointer to the symbol or NULL if not found
 */
struct symbol *find_symbol_used_in(struct queue *table, unsigned used) {
	/* traverse the table */
	struct item *it = table->tail;
	while (it != NULL) {
		struct symbol *sym = (struct symbol *)it->data;
		if (sym->num_used > 0) {
			/* traverse the symbol's 'used' array */
			unsigned i;
			for (i = 0; i < sym->num_used; i++)
				if (sym->used[i] == used)
					/* found the symbol */
					return sym;
		}
		it = it->next;
	}
	/* not found, return NULL */
	return NULL;
}

/**
 * Finds the symbol that is defined in a code line
 * 
 * @param table - pointer to the symbol table (i.e. queue)
 * @param defined - the number of the code line
 * @return	pointer to the symbol or NULL if not found
 */
struct symbol *find_symbol_defined_in(struct queue *table, int defined) {
	/* traverse the table */
	struct item *it = table->tail;
	while (it != NULL) {
		struct symbol *sym = (struct symbol *)it->data;
		if (sym->defined == defined)
			/* found the symbol */
			return sym;
		it = it->next;
	}
	/* not found, return NULL */
	return NULL;
}
