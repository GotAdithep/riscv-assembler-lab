#include "encode.h"

/* Pack R-type instruction */
uint32_t encode_r(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode)
{
    return ((funct7 & 0x7f) << 25) |
           ((rs2 & 0x1f) << 20) |
           ((rs1 & 0x1f) << 15) |
           ((funct3 & 0x7) << 12) |
           ((rd & 0x1f) << 7) |
           (opcode & 0x7f);
}

/* Pack I-type instruction (12-bit immediate) */
uint32_t encode_i(int32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode)
{
    uint32_t uimm = (uint32_t)imm & 0xfffU;
    return (uimm << 20) |
           ((rs1 & 0x1f) << 15) |
           ((funct3 & 0x7) << 12) |
           ((rd & 0x1f) << 7) |
           (opcode & 0x7f);
}

/* Pack S-type instruction (12-bit immediate split) */
uint32_t encode_s(int32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode)
{
    uint32_t uimm = (uint32_t)imm & 0xfffU;
    uint32_t imm_hi = (uimm >> 5) & 0x7f; /* bits 11:5 */
    uint32_t imm_lo = uimm & 0x1f;        /* bits 4:0 */
    return (imm_hi << 25) |
           ((rs2 & 0x1f) << 20) |
           ((rs1 & 0x1f) << 15) |
           ((funct3 & 0x7) << 12) |
           (imm_lo << 7) |
           (opcode & 0x7f);
}

/* Pack B-type instruction (13-bit immediate: imm[12|10:5|4:1|11]) */
uint32_t encode_b(int32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode)
{
    uint32_t uimm = (uint32_t)imm & 0x1fffU; /* 13 bits */
    uint32_t bit12 = (uimm >> 12) & 0x1;     /* imm[12] -> bit 31 */
    uint32_t bits10_5 = (uimm >> 5) & 0x3f;  /* imm[10:5] -> bits 30:25 */
    uint32_t bits4_1 = (uimm >> 1) & 0xf;    /* imm[4:1] -> bits 11:8 */
    uint32_t bit11 = (uimm >> 11) & 0x1;     /* imm[11] -> bit 7 */
    return (bit12 << 31) |
           (bits10_5 << 25) |
           ((rs2 & 0x1f) << 20) |
           ((rs1 & 0x1f) << 15) |
           ((funct3 & 0x7) << 12) |
           (bits4_1 << 8) |
           (bit11 << 7) |
           (opcode & 0x7f);
}

/* Pack J-type instruction (21-bit immediate: imm[20|10:1|11|19:12]) */
uint32_t encode_j(int32_t imm, uint32_t rd, uint32_t opcode)
{
    uint32_t uimm = (uint32_t)imm & 0x1fffffU; /* 21 bits */
    uint32_t bit20 = (uimm >> 20) & 0x1;       /* imm[20] -> bit 31 */
    uint32_t bits10_1 = (uimm >> 1) & 0x3ff;   /* imm[10:1] -> bits 30:21 */
    uint32_t bit11 = (uimm >> 11) & 0x1;       /* imm[11] -> bit 20 */
    uint32_t bits19_12 = (uimm >> 12) & 0xff;  /* imm[19:12] -> bits 19:12 */
    return (bit20 << 31) |
           (bits10_1 << 21) |
           (bit11 << 20) |
           (bits19_12 << 12) |
           ((rd & 0x1f) << 7) |
           (opcode & 0x7f);
}

/* Pack U-type instruction (imm placed in bits 31:12) */
uint32_t encode_u(int32_t imm, uint32_t rd, uint32_t opcode)
{
    uint32_t uimm = ((uint32_t)imm) & 0xfffffU; /* bits 31:12 */
    return (uimm << 12) |
           ((rd & 0x1f) << 7) |
           (opcode & 0x7f);
}
