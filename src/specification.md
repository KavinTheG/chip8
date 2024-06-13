# Chip-8 Specfications

## Memory

- 4096 bytes RAM
    - 0x000 to 0xFFF
    - 0x000 to 0x1FF (first 512 bytes)
        - location of original interpreter
        - should not be used by programs
    - 0x200 (512)
        - Most Chip-8 programs start here
    - 0x600 (1536)
        - Some start here
        - intended for the ETI 660 computer 


## Registers

- 16 general purpose 8-bit registers
    - referred to as V1, V2...,VE, VF
- 16-bit register called I
    - store memory addresses 
    - lowest 12 bits are used
- VF should not be used by any program
    - used as a flag
- PC is the program counter 16 bit register
    - stores the currently executing address
- SP is the stack pointer, 8-bit register
- Stack is an array of 16 16-bit values
    - stores the address that the the interpreter should return to when finishes with a subroutine
    - Chip8 allows for 16 levels of nested subroutines