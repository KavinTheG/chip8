#include "cpu.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/stat.h>

// 4K byte memory
unsigned char memory[4096] = {0};

// 64 x 32 display
unsigned char display[2048] = {0};

// Keypad
unsigned char keys[16] = {0};

unsigned short x, y, px, height;

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

    // Initialize timers
    cpu->dt_register = 0x0;
    cpu->st_register = 0x0;

    // Load fontset
    memcpy(&memory[0x50], chip8_fontset, 80);
}

void load_rom(Chip8 *cpu, char *filename) {
    
    FILE* ptr;

    // Open file in reading mode
    ptr = fopen(filename, "rb");

    if (NULL == ptr)
        printf("NULL file.\n");

    struct stat st;

    stat(filename, &st);

    size_t file_size = st.st_size;

    size_t bytes_read = fread(memory + 0x200, 1, sizeof(memory) - 0x200, ptr);

    fclose(ptr);

    if (bytes_read != file_size)
        printf("File not read.\n");

    
}

void cpu_execute(Chip8 *cpu)
{

    // Fetch opcode from memory at program counter
    cpu->opcode = (memory[cpu->pc_register] << 8) | memory[cpu->pc_register + 1];
    cpu->pc_register += 2;
    cpu->draw_flag = 0;

    switch (cpu->opcode & 0xF000)
    {

    // 0x0XXX
    // Could be CLS, RET, or SYS
    case 0x0000:
        switch (cpu->opcode & 0x0FFF)
        {

        case 0x00E0:
            // CLS - 0x00e0
            fprintf(stdout, "Executing CLS (0x00E0)\n");

            // Clear the display
            cpu->draw_flag = 1;
            for (int i = 0; i < 2048; i++)
            {
                display[i] = 0;
            }
            break;

        case 0x00EE:
            // RET - 0x00ee
            // Return from subroutine
            fprintf(stdout, "Executing RET (0x00EE)\n");

            // Sets program counter to address on top of stack
            cpu->pc_register = cpu->stack[cpu->sp_register];

            // Decrease stack pointer
            cpu->sp_register -= 1;
            break;

        default:
            // SYS addr - 0x0nnn
            fprintf(stdout, "Executing SYS (0x0nnn) with address %x\n", cpu->opcode & 0x0FFF);

            // Sets program counter to nnn
            cpu->pc_register = cpu->opcode & 0x0FFF;
            break;
        }
        break;

    case 0x1000:
        // JP addr - 0x1nnn
        // Jump to instruction at memory location nnn
        fprintf(stdout, "Executing JP (0x1nnn) to address %x\n", cpu->opcode & 0x0FFF);
        cpu->pc_register = cpu->opcode & 0x0FFF;
        break;

    case 0x2000:
        // CALL addr - 0x2nnn
        // Call subroutine at memory location nnn
        fprintf(stdout, "Executing CALL (0x2nnn) to subroutine at address %x\n", cpu->opcode & 0x0FFF);

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
        fprintf(stdout, "Executing SE (0x3xkk) on register V%x with value %x\n",
                (cpu->opcode & 0x0F00) >> 8, cpu->opcode & 0x00FF);

        cpu->pc_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8] == (cpu->opcode & 0x00FF) ? 2 : 0;
        break;

    case 0x4000:
        // SNE Vx, byte - 0x4xkk
        // Skip next instruction if Vx != kk
        fprintf(stdout, "Executing SNE (0x4xkk) on register V%x with value %x\n",
                (cpu->opcode & 0x0F00) >> 8, cpu->opcode & 0x00FF);

        cpu->pc_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8] != (cpu->opcode & 0x00FF) ? 2 : 0;
        break;

    case 0x5000:
        // SE Vx, Vy - 0x5xy0
        // Skip next instruction if Vx = Vy
        fprintf(stdout, "Executing SE (0x5xy0) on registers V%x and V%x\n",
                (cpu->opcode & 0x0F00) >> 8, (cpu->opcode & 0x00F0) >> 4);

        cpu->pc_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8] == cpu->vx[(cpu->opcode & 0x00F0) >> 4] ? 2 : 0;
        break;

    case 0x6000:
        // LD Vx, byte - 0x6xkk
        // Set Vx = kk
        fprintf(stdout, "Executing LD (0x6xkk) on register V%x with value %x\n",
                (cpu->opcode & 0xF00) >> 8, cpu->opcode & 0x0FF);

        cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->opcode & 0x0FF;
        break;

    case 0x7000:
        // ADD Vx, byte - 0x7xkk
        // Set Vx = Vx + kk
        fprintf(stdout, "Executing ADD (0x7xkk) on register V%x with value %x\n",
                (cpu->opcode & 0xF00) >> 8, cpu->opcode & 0x0FF);

        cpu->vx[(cpu->opcode & 0xF00) >> 8] += cpu->opcode & 0x0FF;
        break;

    case 0x8000:
        switch (cpu->opcode & 0xF00F)
        {

        case 0x8000:
            // LD Vx, Vy - 0x8xy0
            // Set Vx = Vy
            fprintf(stdout, "Executing LD (0x8xy0) on registers V%x and V%x\n",
                    (cpu->opcode & 0xF00) >> 8, (cpu->opcode & 0x0F0) >> 4);

            cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->vx[(cpu->opcode & 0x0F0) >> 4];
            break;

        case 0x8001:
            // OR Vx, Vy - 0x8xy1
            // Set Vx = Vx OR Vy
            fprintf(stdout, "Executing OR (0x8xy1) on registers V%x and V%x\n",
                    (cpu->opcode & 0xF00) >> 8, (cpu->opcode & 0x0F0) >> 4);

            cpu->vx[(cpu->opcode & 0xF00) >> 8] |= cpu->vx[(cpu->opcode & 0x0F0) >> 4];
            break;

        case 0x8002:
            // AND Vx, Vy - 0x8xy2
            // Set Vx = Vx AND Vy
            fprintf(stdout, "Executing AND (0x8xy2) on registers V%x and V%x\n",
                    (cpu->opcode & 0xF00) >> 8, (cpu->opcode & 0x0F0) >> 4);

            cpu->vx[(cpu->opcode & 0xF00) >> 8] &= cpu->vx[(cpu->opcode & 0x0F0) >> 4];
            break;

        case 0x8003:
            // XOR Vx, Vy - 0x8xy3
            // Set Vx = Vx XOR Vy
            fprintf(stdout, "Executing XOR (0x8xy3) on registers V%x and V%x\n",
                    (cpu->opcode & 0xF00) >> 8, (cpu->opcode & 0x0F0) >> 4);

            cpu->vx[(cpu->opcode & 0xF00) >> 8] ^= cpu->vx[(cpu->opcode & 0x0F0) >> 4];
            break;

        case 0x8004:
            // ADD Vx, Vy - 0x8xy4
            // Set Vx = Vx + Vy
            fprintf(stdout, "Executing ADD (0x8xy4) on registers V%x and V%x\n",
                    (cpu->opcode & 0xF00) >> 8, (cpu->opcode & 0x0F0) >> 4);

            cpu->vx[(cpu->opcode & 0xF00) >> 8] += cpu->vx[(cpu->opcode & 0x0F0) >> 4];
            break;

        case 0x8005:
            // SUB Vx, Vy - 0x8xy5
            // Set Vx = Vx - Vy and set VF = NOT borrow
            fprintf(stdout, "Executing SUB (0x8xy5) on registers V%x and V%x\n",
                    (cpu->opcode & 0xF00) >> 8, (cpu->opcode & 0x0F0) >> 4);

            cpu->vx[(cpu->opcode & 0xF00) >> 8] -= cpu->vx[(cpu->opcode & 0x0F0) >> 4];

            // VF is set to 1 if there's no borrow, otherwise 0
            cpu->vx[15] = cpu->vx[(cpu->opcode & 0xF00) >> 8] > cpu->vx[(cpu->opcode & 0x0F0) >> 4] ? 1 : 0;
            break;

        case 0x8006:
            // SHR Vx {, Vy} - 0x8xy6
            fprintf(stdout, "Executing SHR (0x8xy6) on register V%x\n",
                    (cpu->opcode & 0xF00) >> 8);

            // VF is set to the least significant bit of Vx
            cpu->vx[15] = cpu->vx[(cpu->opcode & 0xF00) >> 8] & 0x01;

            // Set Vx = Vx >> 1, i.e. divide Vx by 2
            cpu->vx[(cpu->opcode & 0xF00) >> 8] >>= 1;
            break;

        case 0x8007:
            // SUBN Vx, Vy - 0x8xy7
            // Set Vx = Vy - Vx
            fprintf(stdout, "Executing SUBN (0x8xy7) on registers V%x and V%x\n",
                    (cpu->opcode & 0xF00) >> 8, (cpu->opcode & 0x0F0) >> 4);

            cpu->vx[(cpu->opcode & 0xF00) >> 8] = cpu->vx[(cpu->opcode & 0x0F0) >> 4] - cpu->vx[(cpu->opcode & 0xF00) >> 8];
            break;

        case 0x800E:
            // SHL Vx {, Vy} - 0x8xyE
            fprintf(stdout, "Executing SHL (0x8xyE) on register V%x\n",
                    (cpu->opcode & 0xF00) >> 8);

            // VF is set to the most significant bit of Vx
            cpu->vx[15] = cpu->vx[(cpu->opcode & 0xF00) >> 8] & 0x80;

            // Set Vx = Vx << 1, i.e. multiply Vx by 2
            cpu->vx[(cpu->opcode & 0xF00) >> 8] <<= 1;
            break;
        }
        break;

    case 0x9000:
        // SNE Vx, Vy - 0x9xy0
        // Skip next instruction if Vx != Vy
        fprintf(stdout, "Executing SNE (0x9xy0) on registers V%x and V%x\n",
                (cpu->opcode & 0x0F00) >> 8, (cpu->opcode & 0x00F0) >> 4);

        cpu->pc_register += (cpu->vx[(cpu->opcode & 0x0F00) >> 8] != cpu->vx[(cpu->opcode & 0x00F0) >> 4]) ? 2 : 0;
        break;

    case 0xA000:
        // LD I, addr - 0xAnnn
        // Set I = nnn
        fprintf(stdout, "Executing LD I (0xAnnn) with address %x\n", cpu->opcode & 0x0FFF);

        cpu->index_register = cpu->opcode & 0x0FFF;
        break;

    case 0xB000:
        // JP V0, addr - 0xBnnn
        // Jump to location nnn + V0
        fprintf(stdout, "Executing JP (0xBnnn) with address %x and V0 value %x\n",
                cpu->opcode & 0x0FFF, cpu->vx[0]);

        cpu->pc_register = cpu->opcode & 0x0FFF + cpu->vx[0];
        break;

    case 0xC000:
        // RND Vx, byte - 0xCxkk
        // Set Vx = random byte AND kk
        fprintf(stdout, "Executing RND (0xCxkk) on register V%x with random value %x AND %x\n",
                (cpu->opcode & 0x0F00) >> 8, rand() % 255, cpu->opcode & 0x00FF);

        cpu->vx[(cpu->opcode & 0x0F00) >> 8] = (rand() % 255) & (cpu->opcode & 0x00FF);
        break;

    case 0xD000:
        // DRW Vx, Vy, nibble - 0xDxyn
        // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        fprintf(stdout, "Executing DRW (0xDxyn) to display sprite starting at (V%x, V%x) with height %x\n",
                cpu->vx[(cpu->opcode & 0x0F00) >> 8], cpu->vx[(cpu->opcode & 0x00F0) >> 4], cpu->opcode & 0x000F);

        // Get x and y values
        x = cpu->vx[(cpu->opcode & 0x0F00) >> 8];
        y = cpu->vx[(cpu->opcode & 0x00F0) >> 4];
        px = 0;

        // Get height of sprite from n value
        height = cpu->opcode & 0x000F;
        // Set collision flag to 0
        cpu->vx[15] = 0;

        // Loop through height
        for (int i = 0; i < height; i++)
        {

            // Get pixel value from memory at location I
            px = memory[cpu->index_register + i];

            // Loop through width
            for (int j = 0; j < 8; j++)
            {

                if ((px & (0x80 >> j)) != 0)
                {
                    if (display[(x + j + ((y + i) * 64))] == 1)
                    {
                        cpu->vx[15] = 1;
                    }

                    // XORing 1 with the pixel value
                    display[(x + j + ((y + i) * 64))] ^= 1;
                }
            }
        }

        cpu->draw_flag = 1;
        break;

    case 0xE000:

        switch (cpu->opcode & 0x00FF)
        {

        case 0x009E:
            fprintf(stdout, "Executing SKP (0xEx9E) to skip next instruction if key in V%x is pressed\n",
                    (cpu->opcode & 0x0F00) >> 8);

            break;

        case 0x00A1:
            fprintf(stdout, "Executing SKNP (0xExA1) to skip next instruction if key in V%x is not pressed\n",
                    (cpu->opcode & 0x0F00) >> 8);

            break;
        }
        break;

    case 0xF000:

        switch (cpu->opcode & 0x00FF)
        {

        case 0x0007:
            // LD Vx, DT - 0xFx07
            // Set Vx = delay timer value
            fprintf(stdout, "Executing LD (0xFx07) to load delay timer value into V%x\n",
                    (cpu->opcode & 0x0F00) >> 8);

            cpu->vx[(cpu->opcode & 0x0F00) >> 8] = cpu->dt_register;
            break;

        case 0x000A:
            // LD Vx, K - 0xFx0A
            // Wait for a key press, store the value of the key in Vx
            fprintf(stdout, "Executing LD (0xFx0A) to wait for key press and store in V%x\n",
                    (cpu->opcode & 0x0F00) >> 8);

            // Implement key press handling here
            break;

        case 0x0015:
            // LD DT, Vx - 0xFx15
            // Set delay timer = Vx
            fprintf(stdout, "Executing LD (0xFx15) to set delay timer to value in V%x\n",
                    (cpu->opcode & 0x0F00) >> 8);

            cpu->dt_register = cpu->vx[(cpu->opcode & 0x0F00) >> 8];
            break;

        case 0x0018:
            // LD ST, Vx - 0xFx18
            // Set sound timer = Vx
            fprintf(stdout, "Executing LD (0xFx18) to set sound timer to value in V%x\n",
                    (cpu->opcode & 0x0F00) >> 8);

            cpu->st_register = cpu->vx[(cpu->opcode & 0x0F00) >> 8];
            break;

        case 0x001E:
            // ADD I, Vx - 0xFx1E
            // Set I = I + Vx
            fprintf(stdout, "Executing ADD (0xFx1E) to add value in V%x to index register I\n",
                    (cpu->opcode & 0x0F00) >> 8);

            cpu->index_register += cpu->vx[(cpu->opcode & 0x0F00) >> 8];
            break;

        case 0x0029:
            // LD F, Vx - 0xFx29
            // Set I = location of sprite for digit Vx
            fprintf(stdout, "Executing LD (0xFx29) to set I to location of sprite for digit V%x\n",
                    (cpu->opcode & 0x0F00) >> 8);

            // Implement sprite location calculation here
            break;

        case 0x0033:
            // LD B, Vx - 0xFx33
            // Store BCD representation of Vx in memory locations I, I+1, and I+2
            fprintf(stdout, "Executing LD (0xFx33) to store BCD representation of V%x in memory\n",
                    (cpu->opcode & 0x0F00) >> 8);

            // Implement BCD conversion and storage here
            break;

        case 0x0055:
            // LD [I], Vx - 0xFx55
            // Store registers V0 through Vx in memory starting at location I
            fprintf(stdout, "Executing LD (0xFx55) to store registers V0 to V%x in memory\n",
                    (cpu->opcode & 0x0F00) >> 8);

            for (int i = 0; i <= ((cpu->opcode & 0x0F00) >> 8); i++)
            {
                memory[cpu->index_register + i] = cpu->vx[i];
            }
            break;

        case 0x0065:
            // LD Vx, [I] - 0xFx65
            // Read registers V0 through Vx from memory starting at location I
            fprintf(stdout, "Executing LD (0xFx65) to read registers V0 to V%x from memory\n",
                    (cpu->opcode & 0x0F00) >> 8);

            for (int i = 0; i <= ((cpu->opcode & 0x0F00) >> 8); i++)
            {
                cpu->vx[i] = memory[cpu->index_register + i];
            }
            break;

            break;
        }
    }

    if (cpu->dt_register > 0)
    {
        cpu->dt_register--;
    }

    if (cpu->st_register > 0)
    {
        cpu->sound_flag = 1;
        cpu->st_register--;
    }
}
