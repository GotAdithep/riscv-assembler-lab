.text
.globl _start
_start:
    addi x10, x0, 0      # 1: i = 0 (loop counter)
    addi x11, x0, 5      # 2: count = 5
    lui x12, 0x10000     # 3: base address A
    lui x13, 0x10001     # 4: base address B
    lui x14, 0x10002     # 5: base address C
loop:
    beq x10, x11, end    # 6: if i == count, go to end
    slli x15, x10, 2     # 7: offset = i * 4
    add x16, x12, x15    # 8: current addr A
    add x17, x13, x15    # 9: current addr B
    add x18, x14, x15    # 10: current addr C
    lw x19, 0(x16)       # 11: load A[i]
    lw x20, 0(x17)       # 12: load B[i]
    add x21, x19, x20    # 13: A[i] + B[i]
    sub x22, x19, x20    # 14: arithmetic test: sub
    and x23, x19, x20    # 15: logical test: and
    or x24, x19, x20     # 16: logical test: or
    xor x25, x19, x20    # 17: logical test: xor
    sw x21, 0(x18)       # 18: store result in C[i]
    addi x10, x10, 1     # 19: i++
    jal x0, loop         # 20: jump back to loop
end:
    jalr x0, ra, 0       # 21: return/exit