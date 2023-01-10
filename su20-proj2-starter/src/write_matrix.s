.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
#   If any file operation fails or doesn't write the proper number of bytes,
#   exit the program with exit code 1.
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
#
# If you receive an fopen error or eof, 
# this function exits with error code 53.
# If you receive an fwrite error or eof,
# this function exits with error code 54.
# If you receive an fclose error or eof,
# this function exits with error code 55.
# ==============================================================================
write_matrix:
    #errorhandle
    j errskip
error53:
    li a1 53
    jal exit2
error54:
    li a1 54
    jal exit2
error55:
    li a1 55 
    jal exit2
errskip:
    # Prologue
    addi sp sp -24
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw ra 20(sp)

    mv s1 a2
    mv s2 a3 
    mv s3 a1

    # fopen 
   mv a1 a0
    li a2 1

    jal ra fopen

    li t0 -1
    beq t0 a0 error53

    mv s0 a0 # file discriptor

    # write row and col 

    addi sp sp -8
    sw s1 0(sp)
    sw s2 4(sp)

    mv a1 s0
    mv a2 sp
    li a3 2
    li a4 4

    jal ra fwrite

    bne a0 a3 error54
    addi sp sp 8



    # fwrite matrix 
    mv a1 s0
    mv a2 s3
    mul a3 s1 s2
    li a4 4

    jal ra fwrite

    
    bne a0 a3 error54
   
    # fclose
    addi sp sp -4
    sw ra 0(sp)
    
    mv a1 s0

    jal ra fclose

    bne a0 x0 error55

    lw ra 0(sp)
    addi sp sp 4

    # Epilogue
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)   
    lw ra 20(sp)
    addi sp sp 24


    ret
