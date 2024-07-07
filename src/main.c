#include <stdio.h>
#include <unistd.h>

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

        // Handle keypress
        handle_keypress(keys, &running);

        // Draw display
        if (draw_flag == 1)
            draw_display(display);

        usleep(1500);
    }

    quit_graphics();
    return 0;
}
