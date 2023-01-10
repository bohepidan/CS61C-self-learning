.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
#
# If the length of the vector is less than 1, 
# this function exits with error code 5.
# If the stride of either vector is less than 1,
# this function exits with error code 6.
# =======================================================
dot:

    # Prologue
    addi sp sp -12
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)

    j dot_errskip
error5:
    addi a1 x0 5
    jal exit2
error6:
    addi a1 x0 6
    jal exit2

    #error handel
dot_errskip:


    addi t0 x0 1
    blt a2 t0 error5 
    blt a3 t0 error6
    blt a4 t0 error6

loop_start:
    add s0 x0 x0
    add s2 x0 x0 # s2 == sum
    mv s1 a2

loop_continue:
    slli t0 s0 2 
    mul t1 t0 a3
    mul t2 t0 a4

    add t1 t1 a0
    add t2 t2 a1 

    lw t3 0(t1)
    lw t4 0(t2)

    mul t3 t3 t4

    add s2 s2 t3 

    addi s0 s0 1
    blt s0 s1 loop_continue

loop_end:
    mv a0 s2


    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    addi sp sp 12

    
    ret