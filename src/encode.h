#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

/* R-type: funct7 (7), rs2 (5), rs1 (5), funct3 (3), rd (5), opcode (7) */
uint32_t encode_r(uint32_t funct7, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode);

/* I-type: imm[11:0], rs1, funct3, rd, opcode */
uint32_t encode_i(int32_t imm, uint32_t rs1, uint32_t funct3, uint32_t rd, uint32_t opcode);

/* S-type: imm[11:0] split into imm[11:5] and imm[4:0] */
uint32_t encode_s(int32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode);

/* B-type: imm[12:0] encoded with bits at 31,30:25,11:8,7 */
uint32_t encode_b(int32_t imm, uint32_t rs2, uint32_t rs1, uint32_t funct3, uint32_t opcode);

/* J-type: imm[20:0] encoded with bits at 31,30:21,20,19:12 */
uint32_t encode_j(int32_t imm, uint32_t rd, uint32_t opcode);
/* Pack U-type instruction (LUI/AUIPC): imm occupies bits 31:12 */
uint32_t encode_u(int32_t imm, uint32_t rd, uint32_t opcode);

#endif /* ENCODE_H */
