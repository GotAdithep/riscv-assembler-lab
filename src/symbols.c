#include "symbols.h"

#include <string.h>

#define MAX_SYMBOLS 1024
#define MAX_NAME_LEN 63

typedef struct
{
    char name[MAX_NAME_LEN + 1];
    unsigned addr;
} symbol_t;

static symbol_t table[MAX_SYMBOLS];
static size_t table_count = 0;

void symbols_reset(void)
{
    table_count = 0;
}

size_t symbols_count(void)
{
    return table_count;
}

int symbols_add(const char *name, unsigned addr)
{
    if (!name)
        return -1;
    /* check duplicate */
    for (size_t i = 0; i < table_count; ++i)
    {
        if (strcmp(table[i].name, name) == 0)
            return -1; /* duplicate */
    }
    if (table_count >= MAX_SYMBOLS)
        return -1;
    strncpy(table[table_count].name, name, MAX_NAME_LEN);
    table[table_count].name[MAX_NAME_LEN] = '\0';
    table[table_count].addr = addr;
    table_count++;
    return 0;
}

int symbols_find(const char *name, unsigned *addr_out)
{
    if (!name)
        return -1;
    for (size_t i = 0; i < table_count; ++i)
    {
        if (strcmp(table[i].name, name) == 0)
        {
            if (addr_out)
                *addr_out = table[i].addr;
            return 1;
        }
    }
    return 0;
}
