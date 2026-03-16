#include <stdio.h>
#include <stdlib.h>

/* include the header from the src directory */
#include "../src/registers.h"

int main(void)
{
    int failures = 0;

    struct
    {
        const char *name;
        int expect;
    } cases[] = {
        {"x0", 0},
        {"x1", 1},
        {"x31", 31},
        {"x32", -1},
        {"y1", -1},
        {"x", -1},
        {"x-1", -1},
        {"x01", 1},
        {NULL, 0}};

    for (int i = 0; cases[i].name != NULL; ++i)
    {
        int r = parse_register(cases[i].name);
        if (r != cases[i].expect)
        {
            fprintf(stderr, "FAILED: parse_register(%s) == %d, want %d\n",
                    cases[i].name, r, cases[i].expect);
            failures++;
        }
    }

    if (failures == 0)
        printf("All register parse tests passed.\n");

    return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}
