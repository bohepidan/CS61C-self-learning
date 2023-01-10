.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # 
    # If there are an incorrect number of command line args,
    # this function returns with exit code 49.
    #
    # Usage:
    #   main.s -m -1 <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    #   prologue
    addi sp sp -44
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    sw s4 16(sp)
    sw s5 20(sp)
    sw s6 24(sp)
    sw s7 28(sp)
    sw s8 32(sp)
    sw s9 36(sp)
    sw ra 40(sp)

    mv s0 a0 
    mv s1 a1 
    mv s2 a2

    li t0 5
    bne a0 t0 error49
	# =====================================
    # LOAD MATRICES
    # =====================================

    addi sp sp -8
    mv a1 sp 
    addi a2 sp 4
    lw a0 12(s1)
    jal read_matrix

    lw s6 0(sp)
    lw s7 4(sp)
    addi sp sp 8

    mv s3 a0  # ptr to input 

    # Load pretrai 
    addi sp sp -8
    mv a1 sp 
    addi a2 sp 4

    lw a0 4(s1)
    jal read_matrix
    lw s8 0(sp)
    lw s9 4(sp)
    addi sp sp 8

    mv s4 a0  # ptr to m0


    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

    # multipy m0 input
    #[s8, s9] * [s6, s7]

    mul a0 s8 s7 
    slli a0 a0 2 
    jal ra malloc

    mv s5 a0 # matrix d

    mv a0 s4
    mv a1 s8
    mv a2 s9
    mv a3 s3 
    mv a4 s6
    mv a5 s7
    mv a6 s5 

    jal ra matmul

    # free input & m0
    mv a0 s3 
    jal ra free

    mv a0 s4 
    jal ra free

    #   relu 
    mv a0 s5
    mul a1 s8 s7 
    mv s3 a1  # N

    jal ra relu 
    

    # load m1
    addi sp sp -8
    mv a1 sp
    addi a2 sp 4
    lw a0 8(s1)
    jal read_matrix
    lw s6 0(sp)
    lw s7 4(sp)
    addi sp sp 8

    mv s4 a0 # ptr to m1

    # mul m1 relumatrix
    # malloc d2 [a1, a2] * [a4, a5]
    mv a0 s6
    slli a0 a0 2
    jal ra malloc
    
    mv s0 a0 # output


    mv a0 s4
    mv a1 s6 
    mv a2 s7
    mv a3 s5
    mv a4 s3 
    li a5 1
    mv a6 s0 

    jal ra matmul

    # free m1 relu 
    mv a0 s4
    jal ra free

    mv a0 s5 
    jal ra free 
    
    mv s4 s6    #row 
    li s5 1     #col 

    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
    lw a0 16(s1)
    mv a1 s0
    mv a2 s4  
    mv a3 s5 

    jal ra write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax
    mv a0 s0 
    mul a1 s4 s5 

    jal ra argmax

    mv s3 a0  #max index 

    #free output
    mv a0 s0 
    jal ra free 

    # Print classification
    bne x0 s2 leave 

    mv a1 s3 

    jal ra print_int

    # Print newline afterwards for clarity
    li a1 '\n'
    jal ra print_char



leave:

    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    lw s4 16(sp)
    lw s5 20(sp)
    lw s6 24(sp)
    lw s7 28(sp)
    lw s8 32(sp)
    lw s9 36(sp)
    lw ra 40(sp)
    addi sp sp 44

    ret
error49:    
    li a1 49
    jal exit2