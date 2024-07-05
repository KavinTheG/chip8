typedef struct Chip8
{
    unsigned char opcode;

    int index_register;

    int pc_register;

    int sp_register;

    int stack[16];

    unsigned char vx[16];

    unsigned char dt_register;

    unsigned char st_register;

    unsigned char draw_flag;

} Chip8;

// 4K byte memory
unsigned char memory[4096];

// 64 x 32 display
unsigned char display[2048];

// Keypad
unsigned char keys[16];

void load_rom(Chip8 *cpu, char *filename);
void cpu_init(Chip8 *cpu);
void cpu_execute(Chip8 *cpu);
