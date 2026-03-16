#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stddef.h>

/* Simple symbol table for labels -> addresses */
int symbols_add(const char *name, unsigned addr);
/* returns 1 if found (and sets *addr_out if non-NULL), 0 if not found, -1 on error */
int symbols_find(const char *name, unsigned *addr_out);
/* reset table contents */
void symbols_reset(void);
/* current number of symbols */
size_t symbols_count(void);

#endif /* SYMBOLS_H */
