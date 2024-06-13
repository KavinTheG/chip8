typedef struct Chip8 {

    unsigned char opcode;
    
    int index_register;
    
    int pc_register;
    
    int sp_register;

    int stack[16];

    unsigned char vx[16];

    unsigned char dt_register;

    unsigned char st_register;

} Chip8; 