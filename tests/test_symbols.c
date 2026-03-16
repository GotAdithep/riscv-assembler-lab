#include <stdio.h>
#include <stdlib.h>

#include "../src/symbols.h"

int main(void)
{
    symbols_reset();

    if (symbols_add("foo", 0x100) != 0)
    {
        fprintf(stderr, "FAIL: could not add foo\n");
        return 1;
    }
    if (symbols_add("bar", 0x200) != 0)
    {
        fprintf(stderr, "FAIL: could not add bar\n");
        return 1;
    }

    unsigned addr = 0;
    if (symbols_find("foo", &addr) != 1 || addr != 0x100)
    {
        fprintf(stderr, "FAIL: foo lookup wrong (got 0x%x)\n", addr);
        return 1;
    }

    if (symbols_find("baz", &addr) != 0)
    {
        fprintf(stderr, "FAIL: baz unexpectedly found\n");
        return 1;
    }

    /* duplicate insertion should fail */
    if (symbols_add("foo", 0x111) == 0)
    {
        fprintf(stderr, "FAIL: duplicate foo was allowed\n");
        return 1;
    }

    printf("All symbol tests passed.\n");
    return 0;
}
