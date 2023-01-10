.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
#
# If the length of the vector is less than 1, 
# this function exits with error code 8.
# ==============================================================================
# a0 : m0(address) a1 : total(word)
relu:
    li t0 1
    blt a1 t0 error8
    # Prologue
    addi sp, sp, -12
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)

loop_start:
    mv s0, x0


loop_continue:
    slli s1, s0, 2
    add s2, a0, s1  #move ptr
    
    lw t0, 0(s2)
    add t1, x0, x0
    bge t0, t1, reluskip

    sw t1, 0(s2)        #set to zero
reluskip:
    addi s0, s0, 1
    blt s0, a1, loop_continue

loop_end:


    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    addi sp, sp, 12
    
    
	ret
error8:
    li a1 8
    jal exit2