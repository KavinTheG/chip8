#include <cpu.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

unsigned char memory[4096];

// Fonts 
unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D  
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void cpu_init(Chip8 *cpu) {
    cpu->opcode = 0x0;
    cpu->index_register = 0x0;
    cpu->pc_register = 0x200;
    cpu->sp_register = 0x0;
    memcpy(&memory[0x50], chip8_fontset, 80);
}

void cpu_execute(Chip8 *cpu ) {

    // Fetch opcode from memory at program counter
    cpu->opcode = (memory[cpu->pc_register] << 8) | memory[cpu->pc_register + 1];
    cpu->pc_register += 2;

    switch (cpu->opcode & 0xF000) {

        // 0x0XXX
        // Could be CLS, RET, or SYS
        case 0x0000:
            switch (cpu->opcode & 0x0FFF) {

                case 0x00E0:
                    // CLS - 0x00e0

                    // Clear display
                    break;

                case 0x00EE:
                    // RET - 0x00ee
                    // Return from subroutine
                    
                    // Sets program counter to address on top of stack
                    cpu->pc_register = cpu->stack[cpu->sp_register];

                    // Decrease stack pointer
                    cpu->sp_register -= 1;
                    break;

                default:
                    // SYS addr - 0x0nnn

                    // Sets program counter to nnn
                    cpu->pc_register = cpu->opcode & 0x0FFF;
                    break;
            }

            break;

        case 0x1000:
            // JP addr - 0x1nnn
            // Jump to instruction at memory location nnn
            cpu->pc_register = cpu->opcode & 0x0FFF;
            break;

        case 0x2000:
            // CALL addr - 0x2nnn
            // Call subroutine at memory location nnn

            // Increase stack pointer
            cpu->sp_register += 1;

            // Save current program counter on stack
            cpu->stack[cpu->sp_register] = cpu->pc_register;

            // Jump to instruction at memory location nnn
            cpu->pc_register = cpu->opcode & 0x0FFF;
            break;
        
        case 0x3000:
            // SE Vx, byte - 0x3xkk
            // Skip next instruction if Vx = kk

            cpu->pc_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8] == (cpu->opcode & 0x00FF) ? 2 : 0;

        case 0x4000:
            // SNE Vx, byte - 0x4xkk
            // Skip next instruction if Vx != kk

            cpu->pc_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8] != (cpu->opcode & 0x00FF) ? 2 : 0;
            break;

        case 0x5000:
            // SE Vx, Vy - 0x5xy0
            // Skip next instruction if Vx = Vy

            cpu->pc_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8] == cpu->vx[(cpu->opcode & 0x00F0) >> 4] ? 2 : 0;
            break;

        case 0x6000:
            // LD Vx, byte - 0x6xkk
            // Set Vx = kk

            cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->opcode & 0x0FF;
            break;

        case 0x7000:
            // ADD Vx, byte - 0x7xkk
            // Set Vx = Vx + kk

            cpu->vx[(cpu->opcode & 0xF00) >> 8] += cpu->opcode & 0x0FF;
            break;

        case 0x8000:
            switch (cpu->opcode & 0xF00F) {

                case 0x8000:
                    // LD Vx, Vy - 0x8xy0
                    // Set Vx = Vy

                    cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->vx[(cpu->opcode & 0x0F0) >> 4];
                    break;  

                case 0x8001:
                    // OR Vx, Vy - 0x8xy1
                    // Set Vx = Vx OR Vy 

                    cpu->vx[(cpu->opcode & 0xF00) >> 8] |= cpu->vx[(cpu->opcode & 0x0F0) >> 4];
                    break;

                case 0x8002:
                    // AND Vx, Vy - 0x8xy2
                    // Set Vx = Vx AND Vy

                    cpu->vx[(cpu->opcode & 0xF00) >> 8] &= cpu->vx[(cpu->opcode & 0x0F0) >> 4];
                    break;

                case 0x8003:
                    // XOR Vx, Vy - 0x8xy3
                    // Set Vx = Vx XOR Vy

                    cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->vx[(cpu->opcode & 0x0F0) >> 4];
                    break;

                case 0x8004:
                    // ADD Vx, Vy - 0x8xy4
                    // Set Vx = Vx + Vy

                    cpu->vx[(cpu->opcode & 0xF00) >> 8] += cpu->vx[(cpu->opcode & 0x0F0) >> 4];
                    break;

                case 0x8005:
                    // SUB Vx, Vy - 0x8xy5  
                    // Set Vx = Vx - Vy and set VF = NOT borrow

                    cpu->vx[(cpu->opcode & 0xF00) >> 8] -= cpu->vx[(cpu->opcode & 0x0F0) >> 4];

                    // if Vx > Vy then VF = 1 else VF = 0
                    cpu->vx[15] = cpu->vx[(cpu->opcode & 0xF00) >> 8] > cpu->vx[(cpu->opcode & 0x0F0) >> 4] ? 1 : 0;
                    break;

                case 0x8006:
                    // SHR Vx {, Vy} - 0x8xy6
                    
                    // if Vx & 0x01 == 1 then VF = 1 else VF = 0
                    cpu ->vx[15] = cpu->vx[(cpu->opcode & 0xF00) >> 8] & 0x01;

                    // Set Vx = Vx >> 1, i.e. divide Vx by 2
                    cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->vx[(cpu->opcode & 0x0F0) >> 4] >> 1;
                    break;

                case 0x8007:
                    // SUBN Vx, Vy - 0x8xy7
                    // Set Vx = Vy - Vx

                    cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->vx[(cpu->opcode & 0x0F0) >> 4] - cpu->vx[(cpu->opcode & 0xF00) >> 8];
                    break;
                
                case 0x800E:
                    // SHL Vx {, Vy} - 0x8xyE

                    // if Vx & 0x80 == 1 then VF = 1 else VF = 0
                    cpu ->vx[15] = cpu->vx[(cpu->opcode & 0xF00) >> 8] & 0x80;

                    // Set Vx = Vx << 1, i.e. multiply Vx by 2
                    cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->vx[(cpu->opcode & 0x0F0) >> 4] << 1;
                    break;
            }
            break;

        case 0x9000:
            // SNE Vx, Vy - 0x9xy0
            // Skip next instruction if Vx != Vy    

            // Adds 2 to PC if Vx != Vy
            cpu->pc_register += (cpu->vx[(cpu->opcode & 0x0F00) >> 8] != cpu->vx[(cpu->opcode & 0x00F0) >> 4]) ? 2 : 0;
            break;

        case 0xA000:
            // LD I, addr - 0xAnnn
            // Set I = nnn

            cpu->index_register = cpu->opcode & 0x0FFF;
            break; 

        case 0xB000:
            // JP V0, addr - 0xBnnn
            // Jump to location nnn + V0

            cpu->pc_register = cpu->opcode & 0x0FFF + cpu->vx[0];
            break;

        case 0xC000:
            // RND Vx, byte - 0xCxkk
            // Set Vx = random byte AND kk
            
            cpu->vx[(cpu->opcode & 0x0F00) >> 8] = (rand() % 255) & (cpu->opcode & 0x00FF);
            break;

        case 0xD000:
            // DRW Vx, Vy, nibble - 0xDxyn
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision

            break; 
        
        case 0xE000:

            switch (cpu->opcode & 0x00FF) {

                case 0x009E:

                    break;

                case 0x00A1:

                    break;

            }
            break;

        case 0xF000:

            switch (cpu->opcode & 0x00FF) {

                case 0x0007:
                    // LD Vx, DT - 0xFx07
                    // Set Vx = delay timer value

                    cpu->vx[(cpu->opcode & 0x0F00) >> 8] = cpu->dt_register;
                    break;

                case 0x000A:
                    // LD Vx, K - 0xFx0A
                    // Wait for a key press, store the value of the key in Vx
                

                    break;

                case 0x0015:
                    // LD DT, Vx - 0xFx15
                    // Set delay timer = Vx
    
                    cpu->dt_register = cpu->vx[(cpu->opcode & 0x0F00) >> 8];
                    break;

                case 0x0018:
                    // LD ST, Vx - 0xFx18
                    // Set sound timer = Vx

                    cpu->st_register = cpu->vx[(cpu->opcode & 0x0F00) >> 8];
                    break;

                case 0x001E:
                    // ADD I, Vx - 0xFx1E
                    // Set I = I + Vx

                    cpu->index_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8];
                    break;

                case 0x0029:   
                    // LD F, Vx - 0xFx29
                    // Set I = location of sprite for digit Vx


                    break;

                case 0x0033:
                    // LD B, Vx - 0xFx33
                    // Store BCD representation of Vx in memory locations I, I+1, and I+2

                    break;

                case 0x0055:
                    // LD [I], Vx - 0xFx55
                    // Store registers V0 through Vx in memory starting at location I

                    memory[cpu->index_register] = cpu->vx[0];
                    break;

                case 0x0065:
                    // LD Vx, [I] - 0xFx65
                    // Read registers V0 through Vx from memory starting at location I


                    break;

            break;

    }
    
}

