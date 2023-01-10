.import ../../src/read_matrix.s
.import ../../src/utils.s

.data
file_path: .asciiz "inputs/test_read_matrix/test_input.bin"

.text
main:
    # Read matrix into memory
    la a0 file_path
    addi sp sp -8
    addi a1 sp 0
    addi a2 sp 4

    jal ra read_matrix 

    lw a1 0(sp)
    lw a2 4(sp)

    # Print out elements of matrix
    jal ra print_int_array
    
    # Terminate the program
    addi sp sp 8
    jal exit