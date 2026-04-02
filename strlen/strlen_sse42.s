.section .data

.section .text
.globl strlen_sse42
.type strlen_sse42, @function

strlen_sse42:
    # Initialize length counter (%rax) to 0
    xorq %rax, %rax             
    
    # Fill %xmm0 with 0s to act as our null terminator reference
    pxor %xmm0, %xmm0           

.Lloop:
    # Load 16 bytes of the string into %xmm1 
    movdqu (%rdi, %rax), %xmm1  
    
    # FIXED: %xmm1 (the string) is now the source operand.
    # The CPU will now correctly set the Zero Flag ONLY when 
    # it finds a null byte in %xmm1.
    pcmpistri $0x08, %xmm1, %xmm0
    
    # If a null byte is found, the Zero Flag (ZF) is set. Jump to finish.
    jz .Lfound                  
    
    # If no null byte was found, add 16 to our length counter and loop
    addq $16, %rax              
    jmp .Lloop                  

.Lfound:
    # pcmpistri stored the exact index (0-15) of the null byte in %rcx.
    # Add that index to our total length counter.
    addq %rcx, %rax             
    
    # Return the final length
    ret