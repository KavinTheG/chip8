CC = gcc
CFLAGS = -I/opt/homebrew/include -I./include -lSDL2
LDFLAGS = -L/opt/homebrew/lib 
VPATH = src

# Define object files
OBJS = ./build/main.o ./build/cpu.o ./build/peripherals.o

# Default target
all: final

# Link the final executable
final: $(OBJS)
	@echo "Linking and producing the final application"
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o ./bin/chip8

# Compile main.c
main.o: main.c
	@echo "Compiling main.c"
	$(CC) $(CFLAGS) -c $< -o ./build/$@

# Compile cpu.c
cpu.o: cpu.c
	@echo "Compiling cpu.c"
	$(CC) $(CFLAGS) -c $< -o ./build/$@

# Compile peripherals.c
peripherals.o: peripherals.c
	@echo "Compiling peripherals.c"
	$(CC) $(CFLAGS) -c $< -o ./build/$@

# Clean target
clean:
	@echo "Removing all object files"
	rm -r ./build/*.o
