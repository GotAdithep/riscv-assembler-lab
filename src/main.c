#include "assembler.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input.s>\n", argv[0]);
        return 1;
    }
    return assemble_file(argv[1]);
}
