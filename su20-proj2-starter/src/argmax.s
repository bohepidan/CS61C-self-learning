.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
#
# If the length of the vector is less than 1, 
# this function exits with error code 7.
# =================================================================
# a0 = adress, a1 = length
argmax:
    addi sp, sp, -12
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)

    # Prologue

loop_start:
    add s0, x0, x0
    lw t0, 0(a0)  #max val
    mv t1, x0     #max index



loop_continue:
    slli s1, s0, 2
    add s1, s1, a0
    lw s2, 0(s1)
    ble s2, t0, argmaxskip

    add t0, x0, s2
    add t1, x0, s0 

argmaxskip:
    addi s0, s0, 1
    blt s0, a1, loop_continue

loop_end:
    add a0, x0, t1

    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    addi sp, sp, 12

    ret