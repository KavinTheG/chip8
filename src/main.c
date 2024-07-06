#include <stdio.h>
#include "cpu.h"
#include "peripherals.h"
#include "stdbool.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please provide a rom file.\n");
        return 1;
    }

    Chip8 cpu;
    cpu_init(&cpu);
    load_rom(&cpu, argv[1]);

    init_graphics();

    
    bool running = true;
    while (running) {

        // Parse opcode and execute
        cpu_execute(&cpu);
        
        // Draw display
        if (cpu.draw_flag == 1)
            draw_display(&display);

        // Handle keypress
        handle_keypress(&keys, &running);
    }
}
