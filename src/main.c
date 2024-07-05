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
        draw_display(display);

        // Handle keypress
        handle_keypress(&cpu);
    }


}

/*
Pseudocode


main {
    f = read(rom file);
    cpu = chip8();

    for (i = 0; i < f.length; i++) {
        cpu.execute(f[i]);
        cpu.update_timers();
    }

}

*/