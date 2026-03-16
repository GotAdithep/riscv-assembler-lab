#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

#include <stddef.h>

typedef enum {
    FMT_R,
    FMT_I,
    FMT_S,
    FMT_B,
    FMT_U,
    FMT_J
} instr_format_t;

typedef struct instr_entry {
    const char *name;
    instr_format_t fmt;
    unsigned opcode;
    unsigned funct3;
    unsigned funct7;
} instr_entry_t;

/* Lookup an instruction by name. Returns pointer to entry or NULL. */
const instr_entry_t *instr_lookup(const char *name);

/* Number of entries in the table */
size_t instr_table_count(void);

#endif /* INSTR_TABLE_H */
