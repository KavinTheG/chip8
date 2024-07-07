CC = gcc
CFLAGS = -I/opt/homebrew/include -I./include -Wall
LDFLAGS = -L/opt/homebrew/lib -lSDL2 
VPATH = src

# Define object files
OBJS = ./build/main.o ./build/cpu.o ./build/peripherals.o

# Default target
all: final

# Link the final executable
final: $(OBJS)
	@echo "Linking and producing the final application"
	$(CC) $(LDFLAGS) $(OBJS) -o ./bin/chip8

# Compile main.c
./build/main.o: main.c | ./build
	@echo "Compiling main.c"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile cpu.c
./build/cpu.o: cpu.c | ./build
	@echo "Compiling cpu.c"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile peripherals.c
./build/peripherals.o: peripherals.c | ./build
	@echo "Compiling peripherals.c"
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
./build:
	@mkdir -p ./build

# Clean target
clean:
	@echo "Removing all object files"
	rm -r ./build/*.o ./bin/*
