.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
#   The order of error codes (checked from top to bottom):
#   If the dimensions of m0 do not make sense, 
#   this function exits with exit code 2.
#   If the dimensions of m1 do not make sense, 
#   this function exits with exit code 3.
#   If the dimensions don't match, 
#   this function exits with exit code 4.
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0     #
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1    #
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# =======================================================
matmul:

    # Error checks
    j matmul_errskip
error2:
    addi a1 x0 2
    jal exit2
error3:
    addi a1 x0 3
    jal exit2
error4:
    addi a1 x0 4
    jal exit2

matmul_errskip:
    ble a1 x0 error2
    ble a2 x0 error2
    ble a4 x0 error3
    ble a5 x0 error3
    bne a2 a4 error4

    # Prologue
    addi sp sp -16
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)

    add s0 x0 x0

outer_loop_start:

    add s1 x0 x0

inner_loop_start:

    addi sp sp -20
    sw a0 0(sp)
    sw a1 4(sp)
    sw a4 8(sp)
    sw a3 12(sp)
    sw ra 16(sp)


    mul t0 s0 a2 # 
    slli t0 t0 2 #m0 shift
    add a0 a0 t0

    slli t1 s1 2 #m1 shift 
    add a1 t1 a3

    li a3 1
    mv a4 a5

    jal ra dot  # a0 == [s0, s1] 

    mv s3 a0 

    lw a0 0(sp)
    lw a1 4(sp)
    lw a4 8(sp)
    lw a3 12(sp)
    lw ra 16(sp)
    addi sp sp 20

    #write d
    mul s2 s0 a5
    add s2 s2 s1 
    slli s2 s2 2

    add s2 s2 a6 
    sw s3 0(s2)

inner_loop_end:
    addi s1 s1 1
    blt s1 a5 inner_loop_start

outer_loop_end:
    addi s0 s0 1
    blt s0 a1 outer_loop_start

    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    addi sp sp 16
    
    ret