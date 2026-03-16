#include <stdio.h>
#include <stdlib.h>

#include "../src/instr_table.h"

static void expect(const instr_entry_t *e, const char *name, instr_format_t fmt, unsigned opcode, unsigned funct3, unsigned funct7)
{
    if (!e)
    {
        fprintf(stderr, "FAIL: %s not found\n", name);
        exit(1);
    }
    if (e->fmt != fmt || e->opcode != opcode || e->funct3 != funct3 || e->funct7 != funct7)
    {
        fprintf(stderr, "FAIL: %s metadata mismatch\n", name);
        fprintf(stderr, " got fmt=%d opcode=0x%x f3=0x%x f7=0x%x\n", e->fmt, e->opcode, e->funct3, e->funct7);
        fprintf(stderr, "want fmt=%d opcode=0x%x f3=0x%x f7=0x%x\n", fmt, opcode, funct3, funct7);
        exit(1);
    }
}

int main(void)
{
    const instr_entry_t *e;

    e = instr_lookup("add");
    expect(e, "add", FMT_R, 0x33, 0x0, 0x00);

    e = instr_lookup("sub");
    expect(e, "sub", FMT_R, 0x33, 0x0, 0x20);

    e = instr_lookup("addi");
    expect(e, "addi", FMT_I, 0x13, 0x0, 0x00);

    e = instr_lookup("lw");
    expect(e, "lw", FMT_I, 0x03, 0x2, 0x00);

    e = instr_lookup("sw");
    expect(e, "sw", FMT_S, 0x23, 0x2, 0x00);

    e = instr_lookup("beq");
    expect(e, "beq", FMT_B, 0x63, 0x0, 0x00);

    e = instr_lookup("jal");
    expect(e, "jal", FMT_J, 0x6F, 0x0, 0x00);

    /* negative test */
    e = instr_lookup("not-an-instr");
    if (e != NULL)
    {
        fprintf(stderr, "FAIL: unexpected lookup of non-instruction\n");
        return 1;
    }

    printf("All instr_table tests passed.\n");
    return 0;
}
