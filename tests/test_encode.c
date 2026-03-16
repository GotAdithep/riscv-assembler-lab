#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../src/encode.h"

static void check(uint32_t got, uint32_t want, const char *msg)
{
    if (got != want)
    {
        fprintf(stderr, "FAIL %s: got 0x%08x want 0x%08x\n", msg, got, want);
        exit(1);
    }
}

int main(void)
{
    /* add x1, x2, x3 -> R-type: funct7=0x00, rs2=3, rs1=2, funct3=0, rd=1, opcode=0x33 */
    uint32_t r = encode_r(0x00, 3, 2, 0x0, 1, 0x33);
    check(r, 0x003100b3, "add encoding");

    /* addi x1, x2, 5 -> I-type: imm=5, rs1=2, funct3=0, rd=1, opcode=0x13 */
    uint32_t i = encode_i(5, 2, 0x0, 1, 0x13);
    check(i, 0x00510093, "addi encoding");

    /* sw x3, 8(x2) -> S-type: imm=8, rs2=3, rs1=2, funct3=2, opcode=0x23 */
    uint32_t s = encode_s(8, 3, 2, 0x2, 0x23);
    uint32_t want_s = ((0u & 0x7f) << 25) | (3u << 20) | (2u << 15) | (0x2u << 12) | (8u << 7) | 0x23u;
    check(s, want_s, "sw encoding");

    /* beq x1,x2,16 -> B-type imm=16, rs2=2, rs1=1, funct3=0, opcode=0x63 */
    uint32_t b = encode_b(16, 2, 1, 0x0, 0x63);
    /* expected computed via encode rules */
    uint32_t want_b = ((0 & 1) << 31) | ((0 & 0x3f) << 25) | (2 << 20) | (1 << 15) | (0 << 12) | (8 << 8) | (0 << 7) | 0x63;
    check(b, want_b, "beq encoding");

    /* jal x1, 4 -> J-type imm=4, rd=1, opcode=0x6f */
    uint32_t j = encode_j(4, 1, 0x6f);
    uint32_t want_j = ((0 & 1) << 31) | ((4 >> 1 & 0x3ff) << 21) | ((0 & 1) << 20) | ((0 & 0xff) << 12) | (1 << 7) | 0x6f;
    check(j, want_j, "jal encoding");

    printf("All encode tests passed.\n");
    return 0;
}
