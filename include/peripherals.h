#include <stdbool.h>

void init_graphics();
void draw_display(unsigned char *display[2048]);
void handle_keypress(unsigned char *keys, bool *running);
void quit_graphics();