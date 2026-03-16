#include "registers.h"

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int parse_register(const char *name)
{
    if (!name)
        return -1;

    /* Expect exactly: 'x' followed by one or more digits, and nothing else */
    if (name[0] != 'x')
        return -1;

    const char *p = name + 1;
    if (*p == '\0')
        return -1;

    /* ensure all remaining chars are digits */
    const char *q = p;
    while (*q)
    {
        if (!isdigit((unsigned char)*q))
            return -1;
        q++;
    }

    errno = 0;
    long val = strtol(p, NULL, 10);
    if (errno != 0)
        return -1;
    if (val < 0 || val > 31)
        return -1;

    return (int)val;
}
