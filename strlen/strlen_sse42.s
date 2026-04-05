# 322587064 Elad Katz

.section .data


.section .text
.globl strlen_sse42
.type strlen_sse42, @function

strlen_sse42:
    pushq %rcx # Save rcx on stack
    xorq %rax, %rax # Zero counter (faster than ‘xorq %rcx, %rcx‘)
    # %rax is the counter for the lenght of the string because in x86-64 functions always return their final answer in %rax saving us a step
    
    pxor %xmm0, %xmm0 # Fill %xmm0 with 0s to act as our null terminator

_strlen_next: # Label for start of the loop
    # Load 16 bytes of the string into xmm1
    movdqu (%rdi, %rax), %xmm1  # %rdi holds starting add of string and %rax holds current offset counter

    # Checks if we hit the null terminator
    pcmpistri $0x08, %xmm1, %xmm0 # If it finds a match flips the ZF and puts the index of the null terminator into %rcx

    jz _strlen_done # If zero, exit loop

    addq $16, %rax # Move offset conuter by another 16 Bytes
    jmp _strlen_next # # Repeat

_strlen_done:
    addq %rcx, %rax # Save the index of the null terminator

    ret 

