# First arg to function is passed in $rdi and return value is passed in $rax

_strlen:
    pushq %rcx # Save rcx on stack
    xorl %ecx, %ecx # Zero counter (faster than ‘xorq %rcx, %rcx‘)
    # #ecx is the counter for the lenght of the string

_strlen_next: # Label for start of the loop
    cmpb $0, (%rdi) # Compare byte at rdi with 0 (null terminator)
    je _strlen_done # If zero, exit loop
    incq %rcx # Increment character count
    incq %rdi # Move to the next character
    jmp _strlen_next # Repeat

_strlen_done:
    movq %rcx, %rax # Move counter to return value (rax)
    popq %rcx # Restore rcx
    ret # Return to caller
````