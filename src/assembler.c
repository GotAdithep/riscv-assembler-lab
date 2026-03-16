#include "assembler.h"

#include "parser.h"
#include "instr_table.h"
#include "symbols.h"
#include "encode.h"
#include "registers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Simple two-pass assembler: PASS1 records labels, PASS2 encodes and prints words. */
int assemble_file(const char *inpath)
{
    if (!inpath)
        return 1;

    FILE *f = fopen(inpath, "r");
    if (!f)
    {
        perror("fopen");
        return 2;
    }

    symbols_reset();
    char line[1024];
    unsigned pc = 0;

    /* PASS 1 */
    while (fgets(line, sizeof(line), f))
    {
        char label[128];
        char mnemonic[128];
        char operands[512];
        /* initialize outputs */
        label[0] = '\0';
        mnemonic[0] = '\0';
        operands[0] = '\0';
        int r = parse_line(line, label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));
        if (r < 0)
        {
            fprintf(stderr, "parse error\n");
            fclose(f);
            return 3;
        }
        if (r == 1)
            continue; /* blank/comment */
        if (label[0])
        {
            if (symbols_add(label, pc) != 0)
            {
                fprintf(stderr, "duplicate label '%s'\n", label);
                fclose(f);
                return 4;
            }
        }
        if (mnemonic[0])
        {
            if (mnemonic[0] == '.')
                continue; /* assembler directive, ignore */
            pc += 4;
        }
    }

    /* PASS 2 */
    rewind(f);
    pc = 0;
    while (fgets(line, sizeof(line), f))
    {
        char label[128];
        char mnemonic[128];
        char operands[512];
        label[0] = '\0';
        mnemonic[0] = '\0';
        operands[0] = '\0';
        int r = parse_line(line, label, sizeof(label), mnemonic, sizeof(mnemonic), operands, sizeof(operands));
        if (r < 0)
        {
            fprintf(stderr, "parse error\n");
            fclose(f);
            return 3;
        }
        if (r == 1)
            continue; /* blank/comment */
        if (!mnemonic[0])
            continue;
        if (mnemonic[0] == '.')
            continue; /* ignore directives like .text */

        const instr_entry_t *ie = instr_lookup(mnemonic);
        if (!ie)
        {
            fprintf(stderr, "unknown mnemonic '%s'\n", mnemonic);
            fclose(f);
            return 5;
        }

        /* make a mutable copy of operands for tokenizing */
        char opcopy[512];
        strncpy(opcopy, operands, sizeof(opcopy) - 1);
        opcopy[sizeof(opcopy) - 1] = '\0';

        uint32_t word = 0;

        if (ie->fmt == FMT_R)
        {
            char *tok = strtok(opcopy, ",");
            if (!tok)
            {
                fclose(f);
                return 6;
            }
            int rd = parse_register(tok);
            tok = strtok(NULL, ",");
            if (!tok)
            {
                fclose(f);
                return 6;
            }
            int rs1 = parse_register(tok);
            tok = strtok(NULL, ",");
            if (!tok)
            {
                fclose(f);
                return 6;
            }
            int rs2 = parse_register(tok);
            word = encode_r(ie->funct7, (uint32_t)rs2, (uint32_t)rs1, ie->funct3, (uint32_t)rd, ie->opcode);
        }
        else if (ie->fmt == FMT_I)
        {
            if (strcmp(mnemonic, "jalr") == 0)
            {
                char *tok = strtok(opcopy, ",");
                int rd = parse_register(tok);
                tok = strtok(NULL, "(");
                if (!tok)
                {
                    fclose(f);
                    return 6;
                }
                int imm = atoi(tok);
                tok = strtok(NULL, ")");
                int rs1 = parse_register(tok);
                word = encode_i(imm, (uint32_t)rs1, ie->funct3, (uint32_t)rd, ie->opcode);
            }
            else
            {
                char *tok = strtok(opcopy, ",");
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
            char *tok = strtok(opcopy, ",");
            int rs2 = parse_register(tok);
            tok = strtok(NULL, "(");
            if (!tok)
            {
                fclose(f);
                return 6;
            }
            int imm = atoi(tok);
            tok = strtok(NULL, ")");
            int rs1 = parse_register(tok);
            word = encode_s(imm, (uint32_t)rs2, (uint32_t)rs1, ie->funct3, ie->opcode);
        }
        else if (ie->fmt == FMT_B)
        {
            char *tok = strtok(opcopy, ",");
            int rs1 = parse_register(tok);
            tok = strtok(NULL, ",");
            int rs2 = parse_register(tok);
            tok = strtok(NULL, ",");
            char *labelname = tok;
            while (labelname && *labelname == ' ')
                labelname++;
            if (!labelname)
            {
                fclose(f);
                return 6;
            }
            unsigned target = 0;
            if (symbols_find(labelname, &target) != 1)
            {
                fprintf(stderr, "label '%s' not found\n", labelname);
                fclose(f);
                return 7;
            }
            int imm = (int)target - (int)pc;
            word = encode_b(imm, (uint32_t)rs2, (uint32_t)rs1, ie->funct3, ie->opcode);
        }
        else if (ie->fmt == FMT_J)
        {
            char *tok = strtok(opcopy, ",");
            int rd = parse_register(tok);
            tok = strtok(NULL, ",");
            char *labelname = tok;
            while (labelname && *labelname == ' ')
                labelname++;
            if (!labelname)
            {
                fclose(f);
                return 6;
            }
            unsigned target = 0;
            if (symbols_find(labelname, &target) != 1)
            {
                fprintf(stderr, "label '%s' not found\n", labelname);
                fclose(f);
                return 7;
            }
            int imm = (int)target - (int)pc;
            word = encode_j(imm, (uint32_t)rd, ie->opcode);
        }
        else if (ie->fmt == FMT_U)
        {
            char *tok = strtok(opcopy, ",");
            int rd = parse_register(tok);
            tok = strtok(NULL, ",");
            int imm = 0;
            if (tok)
                imm = (int)strtol(tok, NULL, 0);
            word = encode_u(imm, (uint32_t)rd, ie->opcode);
        }
        else
        {
            fprintf(stderr, "unsupported instruction format\n");
            fclose(f);
            return 8;
        }

        printf("0x%08x\n", word);
        pc += 4;
    }

    fclose(f);
    return 0;
}
