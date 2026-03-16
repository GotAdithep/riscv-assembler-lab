#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/parser.h"

static void run_case(const char *line,
                     const char *want_label,
                     const char *want_mnemonic,
                     const char *want_operands)
{
    char label[64];
    char mnemonic[64];
    char operands[256];

    int r = parse_line(line, label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));

    if (want_mnemonic == NULL)
    {
        /* expect blank/comment line */
        if (r != 1)
        {
            fprintf(stderr, "FAIL: expected comment/blank for '%s' got r=%d\n", line, r);
            exit(1);
        }
        return;
    }

    if (r != 0)
    {
        fprintf(stderr, "FAIL: expected success for '%s' got r=%d\n", line, r);
        exit(1);
    }

    if (strcmp(label, want_label) != 0)
    {
        fprintf(stderr, "FAIL label: '%s' != '%s' for line '%s'\n", label, want_label, line);
        exit(1);
    }
    if (strcmp(mnemonic, want_mnemonic) != 0)
    {
        fprintf(stderr, "FAIL mnemonic: '%s' != '%s' for line '%s'\n", mnemonic, want_mnemonic, line);
        exit(1);
    }
    if (strcmp(operands, want_operands) != 0)
    {
        fprintf(stderr, "FAIL operands: '%s' != '%s' for line '%s'\n", operands, want_operands, line);
        exit(1);
    }
}

int main(void)
{
    run_case("  # full line comment", "", NULL, NULL);
    run_case("label: add x1, x2, x3", "label", "add", "x1, x2, x3");
    run_case("  sub x4,x5,x6  ", "", "sub", "x4,x5,x6");
    run_case("loop:", "loop", "", "");
    run_case("  beq x1, x2, loop  # jump", "", "beq", "x1, x2, loop");

    printf("All parser tests passed.\n");
    return 0;
}
