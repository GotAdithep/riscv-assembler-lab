.text
.globl _start
_start:
    lui x5, 0x40000      # 1: Source memory base
    lui x6, 0x50000      # 2: Dest memory base
    addi x7, x0, 256     # 3: Copy 256 words
    addi x8, x0, 0       # 4: Index
copy_loop:
    beq x8, x7, begin_ops# 5: Done copying?
    slli x9, x8, 2       # 6: word offset
    add x10, x5, x9      # 7: src addr
    add x11, x6, x9      # 8: dst addr
    lw x12, 0(x10)       # 9: read src
    sw x12, 0(x11)       # 10: write dst
    addi x8, x8, 1       # 11: increment index
    jal x0, copy_loop    # 12: repeat
begin_ops:
    addi x10, x0, 0x1    # 13: Seed value 1
    addi x11, x0, 0x2    # 14: Seed value 2
    # --- Unrolled ALU Operations Block 1 ---
    add x12, x10, x11    # 15
    sub x13, x12, x10    # 16
    and x14, x13, x11    # 17
    or  x15, x14, x10    # 18
    xor x16, x15, x11    # 19
    sll x17, x16, x10    # 20
    srl x18, x17, x11    # 21
    sra x19, x18, x10    # 22
    slt x20, x19, x11    # 23
    sltu x21, x20, x10   # 24
    addi x22, x21, 15    # 25
    slti x23, x22, 10    # 26
    sltiu x24, x23, 5    # 27
    xori x25, x24, 0xFF  # 28
    ori x26, x25, 0x0F   # 29
    andi x27, x26, 0xF0  # 30
    slli x28, x27, 2     # 31
    srli x29, x28, 1     # 32
    srai x30, x29, 1     # 33
    add x31, x30, x12    # 34
    # --- Unrolled ALU Operations Block 2 ---
    add x12, x31, x11    # 35
    sub x13, x12, x10    # 36
    and x14, x13, x11    # 37
    or  x15, x14, x10    # 38
    xor x16, x15, x11    # 39
    sll x17, x16, x10    # 40
    srl x18, x17, x11    # 41
    sra x19, x18, x10    # 42
    slt x20, x19, x11    # 43
    sltu x21, x20, x10   # 44
    addi x22, x21, 15    # 45
    slti x23, x22, 10    # 46
    sltiu x24, x23, 5    # 47
    xori x25, x24, 0xFF  # 48
    ori x26, x25, 0x0F   # 49
    andi x27, x26, 0xF0  # 50
    slli x28, x27, 2     # 51
    srli x29, x28, 1     # 52
    srai x30, x29, 1     # 53
    add x31, x30, x12    # 54
    # --- Register Shuffle ---
    mv x10, x31          # 55: pseudoinstruction (addi x10, x31, 0)
    mv x11, x30          # 56
    mv x12, x29          # 57
    mv x13, x28          # 58
    mv x14, x27          # 59
    mv x15, x26          # 60
    # --- Memory Store Block ---
    lui x6, 0x60000      # 61: New base
    sw x10, 0(x6)        # 62
    sw x11, 4(x6)        # 63
    sw x12, 8(x6)        # 64
    sw x13, 12(x6)       # 65
    sw x14, 16(x6)       # 66
    sw x15, 20(x6)       # 67
    sh x16, 24(x6)       # 68
    sh x17, 26(x6)       # 69
    sb x18, 28(x6)       # 70
    sb x19, 29(x6)       # 71
    # --- Branching Logic Test ---
    addi x20, x0, 50     # 72
branch_test:
    beq x20, x0, end_prg # 73
    bne x20, x10, bl1    # 74
bl1:
    blt x20, x11, bl2    # 75
bl2:
    bge x20, x12, bl3    # 76
bl3:
    bltu x20, x13, bl4   # 77
bl4:
    bgeu x20, x14, bl5   # 78
bl5:
    addi x20, x20, -1    # 79
    jal x0, branch_test  # 80
end_prg:
    # --- Padding with NOPs and minor math to hit 120 lines ---
    addi x0, x0, 0       # 81 (NOP)
    addi x0, x0, 0       # 82
    addi x0, x0, 0       # 83
    addi x0, x0, 0       # 84
    addi x0, x0, 0       # 85
    addi x0, x0, 0       # 86
    addi x0, x0, 0       # 87
    addi x0, x0, 0       # 88
    addi x0, x0, 0       # 89
    addi x0, x0, 0       # 90
    add x1, x2, x3       # 91
    add x4, x5, x6       # 92
    add x7, x8, x9       # 93
    add x10, x11, x12    # 94
    sub x13, x14, x15    # 95
    sub x16, x17, x18    # 96
    sub x19, x20, x21    # 97
    sub x22, x23, x24    # 98
    and x25, x26, x27    # 99
    and x28, x29, x30    # 100
    or x31, x1, x2       # 101
    or x3, x4, x5        # 102
    xor x6, x7, x8       # 103
    xor x9, x10, x11     # 104
    sll x12, x13, x14    # 105
    srl x15, x16, x17    # 106
    sra x18, x19, x20    # 107
    slt x21, x22, x23    # 108
    sltu x24, x25, x26   # 109
    addi x27, x28, 1     # 110
    addi x29, x30, 2     # 111
    slti x31, x1, 3      # 112
    sltiu x2, x3, 4      # 113
    xori x4, x5, 5       # 114
    ori x6, x7, 6        # 115
    andi x8, x9, 7       # 116
    slli x10, x11, 8     # 117
    srli x12, x13, 9     # 118
    srai x14, x15, 10    # 119
    auipc x1, 0          # 120
    jalr x0, x1, 0       # 121: End Execution