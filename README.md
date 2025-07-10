# chip8

This repository contains my code for a CHIP-8 interpreter written in C. This project is capable of interpreting every chip8 instruction and executing the appropriate function. The following features are includes in this project: 

- 4K memory 
- 16 general purpose registers
- 16-bit index register used to store memory addresses
- delay and sound timers
- 16-bit program counter register used to keep track of program instruction
- 2048 element char array used to display graphics in conjunction with the SDL2 library.

## Prerequisite 

The following items are required for this program to function:

- gcc/clang
- SDL2 

## Usage

Proceed through the following steps to make use of this project and play some chip8 games!

Git clone the repository with the following command: 

```
git clone https://github.com/KavinTheG/chip8.git
```

Change directory to my chip8 project directory

```
cd chip8
```

Enter the following command to build the project

```
Make
```

This will create a binary file capable of running chip8 games. You can view this in the ./bin/ folder. I have two chip8 rom files included in ./roms/ which may be ran by executing the following command.

```
bin/chip8 roms/Space\ Invaders\ \[David\ Winter\].ch8
```

### Keyboard mapping 

The two tables below display how I implemented the mapping from the chip8 keypad to a typical keyboard. Essentially the left half of the keyboard is used to play gammes.

| CHIP-8 | CHIP-8 | CHIP-8 | CHIP-8 |
|--------|--------|--------|--------|
| 1      | 2      | 3      | C      |
| 4      | 5      | 6      | D      |
| 7      | 8      | 9      | E      |
| A      | 0      | B      | F      |

| Keyboard | Keyboard | Keyboard | Keyboard |
|----------|----------|----------|----------|
| 1        | 2        | 3        | 4        |
| Q        | W        | E        | R        |
| A        | S        | D        | F        |
| Z        | X        | C        | V        |


## Demo images

### Space Invaders
<img src="https://github.com/KavinTheG/chip8/assets/25258108/b8cb0695-915c-43c7-ad97-57f5825e9a62" width="450">
<img src="https://github.com/KavinTheG/chip8/assets/25258108/88d1357c-08d9-436b-89c0-d41c780c21e2" width="450">

### Tic-Tac-Toe Invaders
<img src="https://github.com/KavinTheG/chip8/assets/25258108/22895628-b2c8-4236-bfbf-12e51248cc96" width="450">
<img src="https://github.com/KavinTheG/chip8/assets/25258108/4e9cb6d8-9f16-41c9-a0f2-b748f641d9a1" width="450">


## Credit 
The following resources provided much guidance in implementing this project. 

- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM





