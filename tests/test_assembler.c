#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../src/parser.h"
#include "../src/instr_table.h"
#include "../src/symbols.h"
#include "../src/encode.h"
#include "../src/registers.h"

/* Small two-pass assemble test harness. */
int main(void)
{
    const char *program[] = {
        "start: add x1, x2, x3",
        "       addi x4, x0, 10",
        "loop:  beq x1, x2, end",
        "       sw x3, 8(x2)",
        "       jal x0, start",
        "end:   add x5, x5, x5",
        NULL};

    /* PASS 1: record labels */
    symbols_reset();
    unsigned pc = 0;
    for (int i = 0; program[i]; ++i)
    {
        char label[64];
        char mnemonic[64];
        char operands[128];
        int r = parse_line(program[i], label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));
        if (r < 0)
        {
            fprintf(stderr, "parse error\n");
            return 2;
        }
        if (label[0])
        {
            if (symbols_add(label, pc) != 0)
            {
                fprintf(stderr, "duplicate label %s\n", label);
                return 2;
            }
        }
        if (mnemonic[0])
            pc += 4;
    }

    /* PASS 2: encode instructions */
    pc = 0;
    for (int i = 0; program[i]; ++i)
    {
        char label[64];
        char mnemonic[64];
        char operands[128];
        int r = parse_line(program[i], label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));
        if (r < 0)
        {
            fprintf(stderr, "parse error\n");
            return 2;
        }
        if (!mnemonic[0])
            continue;

        const instr_entry_t *ie = instr_lookup(mnemonic);
        if (!ie)
        {
            fprintf(stderr, "unknown mnemonic %s\n", mnemonic);
            return 2;
        }

        uint32_t word = 0;

        if (ie->fmt == FMT_R)
        {
            /* rd, rs1, rs2 */
            char *tok = strtok(operands, ",");
            if (!tok)
                return 3;
            int rd = parse_register(tok);
            tok = strtok(NULL, ",");
            if (!tok)
                return 3;
            int rs1 = parse_register(tok);
            tok = strtok(NULL, ",");
            if (!tok)
                return 3;
            int rs2 = parse_register(tok);
            word = encode_r(ie->funct7, (uint32_t)rs2, (uint32_t)rs1, ie->funct3, (uint32_t)rd, ie->opcode);
        }
        else if (ie->fmt == FMT_I)
        {
            /* either rd, rs1, imm  OR jalr rd, imm(rs1) */
            if (strcmp(mnemonic, "jalr") == 0)
            {
                char *tok = strtok(operands, ",");
                int rd = parse_register(tok);
                tok = strtok(NULL, "("); /* imm */
                int imm = atoi(tok);
                tok = strtok(NULL, ")");
                int rs1 = parse_register(tok);
                word = encode_i(imm, (uint32_t)rs1, ie->funct3, (uint32_t)rd, ie->opcode);
            }
            else
            {
                char *tok = strtok(operands, ",");
                int rd = parse_register(tok);
                tok = strtok(NULL, ",");
                int rs1 = parse_register(tok);
                tok = strtok(NULL, ",");
                int imm = atoi(tok);
                word = encode_i(imm, (uint32_t)rs1, ie->funct3, (uint32_t)rd, ie->opcode);
            }
        }
        else if (ie->fmt == FMT_S)
        {
            /* rs2, imm(rs1) */
            char *tok = strtok(operands, ",");
            int rs2 = parse_register(tok);
            tok = strtok(NULL, "(");
            int imm = atoi(tok);
            tok = strtok(NULL, ")");
            int rs1 = parse_register(tok);
            word = encode_s(imm, (uint32_t)rs2, (uint32_t)rs1, ie->funct3, ie->opcode);
        }
        else if (ie->fmt == FMT_B)
        {
            /* rs1, rs2, label */
            char *tok = strtok(operands, ",");
            int rs1 = parse_register(tok);
            tok = strtok(NULL, ",");
            int rs2 = parse_register(tok);
            tok = strtok(NULL, ",");
            char *labelname = tok;
            while (*labelname == ' ')
                labelname++;
            unsigned target = 0;
            if (symbols_find(labelname, &target) != 1)
            {
                fprintf(stderr, "label %s not found\n", labelname);
                return 4;
            }
            int imm = (int)target - (int)pc;
            word = encode_b(imm, (uint32_t)rs2, (uint32_t)rs1, ie->funct3, ie->opcode);
        }
        else if (ie->fmt == FMT_J)
        {
            /* jal rd, label */
            char *tok = strtok(operands, ",");
            int rd = parse_register(tok);
            tok = strtok(NULL, ",");
            char *labelname = tok;
            while (*labelname == ' ')
                labelname++;
            unsigned target = 0;
            if (symbols_find(labelname, &target) != 1)
            {
                fprintf(stderr, "label %s not found\n", labelname);
                return 4;
            }
            int imm = (int)target - (int)pc;
            word = encode_j(imm, (uint32_t)rd, ie->opcode);
        }
        else
        {
            fprintf(stderr, "unsupported format in test\n");
            return 5;
        }

        /* For test validation, recompute using direct encode call and instr entry (already did), so just print */
        printf("0x%08x\n", word);

        pc += 4;
    }

    printf("Assemble test completed.\n");
    return 0;
}
