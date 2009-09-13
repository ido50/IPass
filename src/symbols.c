#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"
#include "symbols.h"

struct symbol *new_symbol(char *name, int defined, unsigned used, int type) {
	struct symbol *new = (struct symbol *)malloc(sizeof(struct symbol));

	new->name = name;
	new->defined = defined;
	new->type = type;
	new->used[new->num_used++] = used;
	new->entry = 0;

	return new;
}

struct symbol *find_symbol(struct queue *table, char *name) {
	struct item *it = table->tail;
	while (it != NULL) {
		struct symbol *sym = (struct symbol *)it->data;
		if (strcmp(sym->name, name) == 0)
			return sym;
		it = it->next;
	}
	return NULL;
}

void update_used(struct symbol *sym, unsigned used) {
	sym->used[sym->num_used++] = used;
}

void update_defined(struct symbol *sym, int defined, int type) {
	sym->defined = defined;
	sym->type = type;
}

struct symbol *find_symbol_used_in(struct queue *table, unsigned used) {
	struct item *it = table->tail;
	while (it != NULL) {
		struct symbol *sym = (struct symbol *)it->data;
		if (sym->num_used > 0) {
			unsigned i;
			for (i = 0; i < sym->num_used; i++)
				if (sym->used[i] == used)
					return sym;
		}
		it = it->next;
	}
	return NULL;
}

struct symbol *find_symbol_defined_in(struct queue *table, int defined) {
	struct item *it = table->tail;
	while (it != NULL) {
		struct symbol *sym = (struct symbol *)it->data;
		if (sym->defined == defined)
			return sym;
		it = it->next;
	}
	return NULL;
}
