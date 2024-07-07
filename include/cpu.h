typedef struct Chip8
{

    // opcode   
    unsigned short opcode;

    // index register
    unsigned short index_register;

    // program counter register
    unsigned short pc_register;

    // stack pointer register
    unsigned short sp_register;

    // stack
    unsigned short stack[16];

    // Gneeral purpose registers
    unsigned char vx[16];

    // Timers
    unsigned char dt_register;

    // Sound
    unsigned char st_register;

} Chip8;

// 4K byte memory
unsigned char memory[4096];

// 64 x 32 display
unsigned char display[2048];

// Keypad
unsigned char keys[16];

// Draw flag
unsigned char draw_flag;

// Sound flag
unsigned char sound_flag;

void load_rom(Chip8 *cpu, char *filename);
void cpu_init(Chip8 *cpu);
void cpu_execute(Chip8 *cpu);
