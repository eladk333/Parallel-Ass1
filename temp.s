_strlen: 
    # This is the label marking the entry point of the function[cite: 34].
    
    pushq %rcx 
    # Saves the current value of the %rcx register onto the stack so it isn't overwritten and lost[cite: 35]. 
    # The caller might be relying on whatever was in %rcx.
    
    xorl %ecx, %ecx 
    # Clears the %ecx register (the lower 32 bits of %rcx) by XORing it with itself, effectively setting the counter to 0[cite: 36]. 
    # Doing this on the 32-bit register automatically zeroes the upper 32 bits of %rcx, and it is a faster operation than moving 0 into the register[cite: 36].

_strlen_next: 
    # A label marking the start of the loop that will check each character[cite: 37].
    
    cmpb $0, (%rdi) 
    # Compares the single byte ('b' for byte) at the memory address currently held in %rdi against 0[cite: 38]. 
    # 0 represents the null terminator ('\0') at the end of a C-string[cite: 38].
    
    je _strlen_done 
    # "Jump if Equal". If the previous comparison was true (the byte is 0), the end of the string has been reached, so jump out of the loop to _strlen_done[cite: 39].
    
    incq %rcx 
    # If it wasn't 0, increment the character counter in %rcx by 1[cite: 40].
    
    incq %rdi 
    # Increment the memory address pointer in %rdi by 1 byte so it points to the next character in the string[cite: 41].
    
    jmp _strlen_next 
    # Unconditionally jump back up to the _strlen_next label to check the newly pointed-to character[cite: 42].

_strlen_done: 
    # A label marking the end of the loop and the start of the return sequence[cite: 43].
    
    movq %rcx, %rax 
    # Moves the final character count from %rcx into %rax[cite: 44]. 
    # In x86-64, %rax is the standard register used to pass the return value back to the caller[cite: 44].
    
    popq %rcx 
    # Restores the original value of %rcx from the stack[cite: 45].
    
    ret 
    # Returns control flow back to the calling function[cite: 46].