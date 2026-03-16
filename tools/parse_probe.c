#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/parser.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 2;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror("fopen");
        return 2;
    }
    char line[1024];
    int lineno = 0;
    while (fgets(line, sizeof(line), f))
    {
        lineno++;
        char label[128], mnemonic[128], operands[512];
        int r = parse_line(line, label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));
        printf("%4d: r=%d | label='%s' | mnemonic='%s' | operands='%s'\n", lineno, r, label, mnemonic, operands);
    }
    fclose(f);
    return 0;
}
