.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#   If any file operation fails or doesn't read the proper number of bytes,
#   exit the program with exit code 1.
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
#
# If you receive an fopen error or eof, 
# this function exits with error code 50.
# If you receive an fread error or eof,
# this function exits with error code 51.
# If you receive an fclose error or eof,
# this function exits with error code 52.
# ==============================================================================
read_matrix:
    #errorhandle
    j errskip
error48:
    li a1 48 
    jal exit2
error50:
    li a1 50 
    jal exit2
error51:
    li a1 51 
    jal exit2
error52:
    li a1 52 
    jal exit2

errskip:
    # Prologue
    addi sp sp -20
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
	
    mv s1 a1
    mv s2 a2

    # fopen
    addi sp sp -16
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw ra 12(sp)

    mv a1 a0
    li a2 0

    jal ra fopen

    li t0 -1
    beq t0 a0 error50

    mv s0 a0 # file discription

    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw ra 12(sp)
    addi sp sp 16
    
    # fread 
    #read row
    addi sp sp -20
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    sw ra 16(sp)
    
    mv a2 a1
    mv a1 s0
    li a3 4
    mv t0 a3

    jal ra fread 


    
    bne a0 t0 error51

    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw a3 12(sp)
    lw ra 16(sp)
    addi sp sp 20
 
    #read col
    addi sp sp -20
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    sw ra 16(sp)
    
    mv a1 s0
    li a3 4
    mv t0 a3

    jal ra fread 
    bne a0 t0 error51


    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw a3 12(sp)
    lw ra 16(sp)
    addi sp sp 20

    # malloc 0(s1) * 0(s2) * 4
    lw t1 0(s1)
    lw t2 0(s2)
    mul a0 t1 t2
    slli a0 a0 2

    addi sp sp -4
    sw ra 0(sp)

    jal malloc
    beq a0 x0 error48 

    mv s3 a0
    lw ra 0(sp)
    addi sp sp 4

    # read matrix 
    addi sp sp -20
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    sw ra 16(sp)
    
    mv a2 s3 
    mv a1 s0
    lw t1 0(s1)
    lw t2 0(s2)

    mul a3 t1 t2 

    slli a3 a3 2
    mv s4 a3

    jal ra fread 
    
    bne a0 s4 error51

    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw a3 12(sp)
    lw ra 16(sp)
    addi sp sp 20
    
    # fclose
    addi sp sp -4
    sw ra 0(sp)
    
    mv a1 s0

    jal ra fclose

    bne a0 x0 error52

    lw ra 0(sp)
    addi sp sp 4

    mv a0 s3

    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    addi sp sp 20

    ret