#define MAXUSED 100 /* Maximum number of times a symbol can be used in a single source file */


struct symbol {
	char *name;			/* symbol's name */
	int defined;			/* number of code line where symbol was defined */
	int type;			/* symbol's type (0 for instruction symbol, 1 for data) */
	unsigned used[MAXUSED];		/* array of code lines that refer to this symbol */
	unsigned num_used;		/* the length of the above array (updated automatically) */
	unsigned entry;			/* set to a true value if an entry is set for this symbol */
};

/**
 * Creates a new symbol
 * 
 * @param name - name of the symbol (i.e. the label)
 * @param defined - the number of the code line where it was defined (use -1 if unknown yet)
 * @param used - the number of the code line where it was first used (use -1 if not yet used)
 * @param type - the type of the symbol (0 for instruction symbol, 1 for data symbol)
 * @return	pointer to the new symbol
 */
struct symbol *new_symbol(char *, int, unsigned, int);

/**
 * Finds a symbol in a symbol table according to name
 * 
 * @param table - pointer to the symbol table (i.e. queue)
 * @param name - name of the symbol (i.e. the label)
 * @return	pointer to the found symbol or NULL if not found
 */
struct symbol *find_symbol(struct queue *, char *);

/**
 * Adds the number of a code line where a symbol was used
 * 
 * @param sym - pointer to the symbol that was used
 * @param used - the number of the code line where it was used
 */
void update_used(struct symbol *, unsigned);

void update_defined(struct symbol *, int, int);

struct symbol *find_symbol_used_in(struct queue *, unsigned);
struct symbol *find_symbol_defined_in(struct queue *, int);
