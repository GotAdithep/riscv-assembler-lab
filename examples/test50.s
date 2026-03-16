.text
.globl _start
_start:
    lui x5, 0x20000      # 1: array base address
    addi x6, x0, 10      # 2: array size (N=10)
    addi x7, x0, 0       # 3: outer loop counter (i=0)
outer_loop:
    bge x7, x6, end_sort # 4: if i >= N, sorting done
    addi x8, x0, 0       # 5: inner loop counter (j=0)
    sub x9, x6, x7       # 6: N - i
    addi x9, x9, -1      # 7: N - i - 1
inner_loop:
    bge x8, x9, next_out # 8: if j >= N-i-1, next outer loop
    slli x10, x8, 2      # 9: offset j * 4
    add x11, x5, x10     # 10: addr of arr[j]
    addi x12, x11, 4     # 11: addr of arr[j+1]
    lw x13, 0(x11)       # 12: load arr[j]
    lw x14, 0(x12)       # 13: load arr[j+1]
    ble x13, x14, no_swp # 14: if arr[j] <= arr[j+1], don't swap
    sw x14, 0(x11)       # 15: swap (store arr[j+1] at j)
    sw x13, 0(x12)       # 16: swap (store arr[j] at j+1)
no_swp:
    addi x8, x8, 1       # 17: j++
    jal x0, inner_loop   # 18: repeat inner loop
next_out:
    addi x7, x7, 1       # 19: i++
    jal x0, outer_loop   # 20: repeat outer loop
end_sort:
    # --- Dummy Math & Memory Exercising ---
    addi x15, x0, 100    # 21: init limit
    addi x16, x0, 1      # 22: init multiplier
math_loop:
    beq x15, x0, finish  # 23: exit if 0
    sll x17, x15, x16    # 24: shift left logical
    srl x18, x15, x16    # 25: shift right logical
    sra x19, x15, x16    # 26: shift right arithmetic
    slt x20, x17, x18    # 27: set less than
    sltu x21, x17, x18   # 28: set less than unsigned
    add x22, x17, x18    # 29: add
    sub x23, x17, x18    # 30: sub
    xor x24, x22, x23    # 31: xor
    or x25, x22, x23     # 32: or
    and x26, x22, x23    # 33: and
    lui x27, 0x30000     # 34: temp memory base
    add x28, x27, x15    # 35: temp address
    sb x24, 0(x28)       # 36: store byte
    sh x25, 2(x28)       # 37: store half-word
    sw x26, 4(x28)       # 38: store word
    lb x29, 0(x28)       # 39: load byte
    lh x30, 2(x28)       # 40: load half-word
    lhu x31, 2(x28)      # 41: load half-word unsigned
    lbu x5, 0(x28)       # 42: load byte unsigned
    addi x15, x15, -1    # 43: decrement loop
    bne x15, x0, m_jump  # 44: branch not equal
    jal x0, finish       # 45: absolute jump to end
m_jump:
    jal x0, math_loop    # 46: loop back
finish:
    auipc x1, 0          # 47: add upper immediate to pc
    addi x1, x1, 12      # 48: setup return address manually
    jalr x0, x1, 0       # 49: jump and link register
    addi x0, x0, 0       # 50: NOP
    addi x0, x0, 0       # 51: NOP