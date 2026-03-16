#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/parser.h"
#include "../src/instr_table.h"
#include "../src/symbols.h"
#include "../src/encode.h"
#include "../src/registers.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s file\n", argv[0]);
        return 2;
    }
    const char *inpath = argv[1];
    FILE *f = fopen(inpath, "r");
    if (!f)
    {
        perror("fopen");
        return 2;
    }
    symbols_reset();
    char line[1024];
    unsigned pc = 0;
    int lineno = 0;
    while (fgets(line, sizeof(line), f))
    {
        lineno++;
        char label[128], mnemonic[128], operands[512];
        int r = parse_line(line, label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));
        if (r < 0)
        {
            fprintf(stderr, "parse err\n");
            return 2;
        }
        if (label[0])
        {
            printf("PASS1: line %d label='%s' pc=%u\n", lineno, label, pc);
            symbols_add(label, pc);
        }
        if (mnemonic[0])
            pc += 4;
    }
    rewind(f);
    pc = 0;
    lineno = 0;
    while (fgets(line, sizeof(line), f))
    {
        lineno++;
        char label[128], mnemonic[128], operands[512];
        int r = parse_line(line, label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));
        if (r < 0)
        {
            fprintf(stderr, "parse err2\n");
            return 2;
        }
        if (!mnemonic[0])
            continue;
        printf("PASS2: line %d mnemonic='%s' operands='%s' pc=%u\n", lineno, mnemonic, operands, pc);
        const instr_entry_t *ie = instr_lookup(mnemonic);
        if (!ie)
        {
            printf("LOOKUP FAIL at line %d mnemonic='%s'\n", lineno, mnemonic);
            /* compute encoding for diagnostics */
        }
        else
        {
            printf("LOOKUP OK name=%s fmt=%d opcode=0x%x\n", ie->name, ie->fmt, ie->opcode);
            /* compute encoding for diagnostics */
            char opcopy[512];
            strncpy(opcopy, operands, sizeof(opcopy) - 1);
            opcopy[sizeof(opcopy) - 1] = '\0';
            uint32_t word = 0;
            if (ie->fmt == FMT_R)
            {
                char *t = strtok(opcopy, ",");
                int rd = parse_register(t);
                t = strtok(NULL, ",");
                int rs1 = parse_register(t);
                t = strtok(NULL, ",");
                int rs2 = parse_register(t);
                word = encode_r(ie->funct7, rs2, rs1, ie->funct3, rd, ie->opcode);
            }
            else if (ie->fmt == FMT_I)
            {
                if (strcmp(ie->name, "jalr") == 0)
                {
                    char *t = strtok(opcopy, ",");
                    int rd = parse_register(t);
                    t = strtok(NULL, "(");
                    int imm = atoi(t);
                    t = strtok(NULL, ")");
                    int rs1 = parse_register(t);
                    word = encode_i(imm, rs1, ie->funct3, rd, ie->opcode);
                }
                else
                {
                    char *t = strtok(opcopy, ",");
                    int rd = parse_register(t);
                    t = strtok(NULL, ",");
                    int rs1 = parse_register(t);
                    t = strtok(NULL, ",");
                    int imm = atoi(t);
                    word = encode_i(imm, rs1, ie->funct3, rd, ie->opcode);
                }
            }
            else if (ie->fmt == FMT_S)
            {
                char *t = strtok(opcopy, ",");
                int rs2 = parse_register(t);
                t = strtok(NULL, "(");
                int imm = atoi(t);
                t = strtok(NULL, ")");
                int rs1 = parse_register(t);
                word = encode_s(imm, rs2, rs1, ie->funct3, ie->opcode);
            }
            else if (ie->fmt == FMT_B)
            {
                char *t = strtok(opcopy, ",");
                int rs1 = parse_register(t);
                t = strtok(NULL, ",");
                int rs2 = parse_register(t);
                t = strtok(NULL, ",");
                char *labelname = t;
                while (labelname && *labelname == ' ')
                    labelname++;
                unsigned target = 0;
                if (symbols_find(labelname, &target) != 1)
                {
                    printf(" label not found\n");
                }
                else
                {
                    int imm = (int)target - (int)pc;
                    word = encode_b(imm, rs2, rs1, ie->funct3, ie->opcode);
                }
            }
            else if (ie->fmt == FMT_J)
            {
                char *t = strtok(opcopy, ",");
                int rd = parse_register(t);
                t = strtok(NULL, ",");
                char *labelname = t;
                while (labelname && *labelname == ' ')
                    labelname++;
                unsigned target = 0;
                if (symbols_find(labelname, &target) != 1)
                {
                    printf(" label not found\n");
                }
                else
                {
                    int imm = (int)target - (int)pc;
                    word = encode_j(imm, rd, ie->opcode);
                }
            }
            else if (ie->fmt == FMT_U)
            {
                char *t = strtok(opcopy, ",");
                int rd = parse_register(t);
                t = strtok(NULL, ",");
                int imm = 0;
                if (t)
                    imm = (int)strtol(t, NULL, 0);
                word = encode_u(imm, rd, ie->opcode);
            }
            if (word)
                printf("  ENCODE: 0x%08x\n", word);
        }
        pc += 4;
    }
    fclose(f);
    return 0;
}
