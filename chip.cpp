#include "chip.h"
#include <fstream>
#include <stdlib.h>
#include <time.h>

/* macro to define the address pointed to by the 
 * program counter upon boot time
 */
#define START_ADDRESS       0x200
#define FONT_START_ADDRESS  0x50

/* 
 * chip8 constructor
 * initializes the system
 * inputs: none
 * outputs: settings initialized like the program counter
 */
chip8::chip8() {
    // initialize the pc for the system
    pc = START_ADDRESS;

    // initialize the font set data into memory
    for (int i = 0; i < FONTSET_SIZE; ++i) {
        memory[FONT_START_ADDRESS + i] = fonntset[i];
    }
}


/* 
 * loadROM function
 * this is to the program file onto the chip8 memory 
 * to be interpreted by the system after
 * inputs: name of the file which holds the program
 * outputs: none
 */
void chip8::loadROM(char const* filename){
    // open the file an dmove the file pointer to the end
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    // if the file is open then proceed
    if (file.is_open()) {
        // get the size of the file
        std::streampos size = file.tellg();

        // make a buffer to hold the file's content
        char* buffer = new char[size];
        // got to the start of the file and copy content 
        // into the tmep buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);

        // close the file
        file.close();

        // copy the file content from the temp buffer into 
        // the chip8 memory
        for (long i = 0; i < size; i++) {
            memory[START_ADDRESS + i] * buffer[i];
        }

        // clean up local variables
        delete[] buffer;
    }
}

/*
 * the following are the implementations of 
 * the opcode instructions for the chip-8
 */

 /*
  * OPcode 00E0
  * clear the display
  */
void chip8::00E0() {
    // set the video memory to hold all 0s
    std::memset(video, 0, VIDMEM_SIZE);
}

/*
 * OPcode 00EE
 * returns fromt he subroutine
 */
void chip8::00EE() {
    // decrement the stack pointer to point to the return 
    // address stored in the previous position
    sp--;

    // update the program counter to point to the correct 
    // instruction after the return
    pc = stack[sp];
}

/*
 * OPcode 1nnn
 * jump to location nnn
 */
void chip8::1nnn() {
    // update pc to hold the instruction address that the 
    // OPcode gives
    pc = opcode & 0x0FFF;
}

/*
 * OPcode 2nnn
 * call instruction - call subroutine at location nnn
 */
void chip8::2nnn() {
    // push the return address (pc) into the stack and increment stack pointer
    stack[sp] = pc;
    sp++;

    // update pc to jump to the called subroutine
    pc = opcode & 0x0FFF;
}

/*
 * OPcode 3xkk
 * skip the next instruction if register Vx holds the value kk
 */
void chip8::3xkk() {
    // get the register nuber from the opcode
    int regnum = (opcode & 0x0F00) >> 8;

    // get the value kk
    uint8_t val = opcode & 0x00FF;

    // check if the register content matches the given value
    if (registers[regnum] == val) {

        // update the pc to skip the next instruction
        // each instruction takes 2 bytes hence the +2
        pc += 2;
    }
}

/*
 * OPcode 4xkk
 * skip next instruction if the value in reg Vx is not equal to kk
 */
void chip8::4xkk() {
    // find the register number from the opcode
    int regnum = (opcode & 0x0F00) >> 8;

    // get the value kk
    uint8_t val = opcode & 0x00FF;

    // check if the reg Vx holds the same value as kk
    if (registers[regnum] != val) {

        // if not update pc to skip next instruction
        pc += 2;
    }
}

/*
 * OPcode 5xy0
 * skip the next instruction if registers Vx and Vy hold the same value
 */
void chip8::5xy0() {
    // get the register numbers from the opcode
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // check if they're the same
    if (registers[x] == registers[y]) {
        // skip next instruction if so
        pc += 2;
    }
}

/*
 * OPcode 6xkk
 * load the value kk into register Vx
 */
void chip8::6xkk() {
    // get the reg number
    int regnum = (opcode & 0x0F00) >> 8;

    // update the value
    registers[regnum] = opcode & 0x00FF;
}

/*
 * OPcode 7xkk
 * add the value kk to the value in register Vx
 */
void chip8::7xkk() {
    // get the reg num and value
    int regnum = (opcode & 0x0F00) >> 8;
    uint8_t val = opcode & 0x00FF;

    // add the value into the register value and store in the register
    registers[regnum] += val;
}

/*
 * OPcode 8xy0
 * load the value in register y into register x (Vx = Vy)
 */
void chip8::8xy0() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // updte reg x's value
    registers[x] = registers[y];
}

/*
 * OPcode 8xy1
 * value in reg x = value in reg x ORed with value in reg y
 */
void chip8::8xy1() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // update reg x's value
    registers[x] |= registers[y];
}

/*
 * OPcode 8xy2
 * bitwise AND of values in reg x and y - stored in reg x
 */
void chip8::8xy2() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // update reg x's value
    registers[x] &= registers[y];
}

/*
 * OPcode 8xy3
 * bitwise XOR of values in reg x and y - stored in reg x
 */
void chip8::8xy3() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // update reg x's value
    registers[x] ^= registers[y];
}

/*
 * OPcode 8xy4
 * reg x = reg x + reg y
 * sum is only 8 bits long, if carry found reg 0xF = 1, else 0
 */
void chip8::8xy4() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // get the sum
    uint16_t sum = registers[x] + registers[y];

    // check for overflow and give reg0xF value accordingly
    registers[0xF] = sum > 255 ? 1 : 0;

    // update reg x's value
    registers[x] = sum & 0x00FF;
}

/*
 * OPcode 8xy5
 * reg x = reg x - reg y
 * if reg x > reg y, reg0xF = 1, else 0
 */
void chip8::8xy5() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // set register0xF's value
    registers[0xF] = registers[x] > registers[y] ? 1 : 0;

    // update regx
    registers[x] -= registers[y];
}

/*
 * OPcode 8xy6
 * right shift reg x's value by 1 (divide by 2)
 * if least significant bit of reg x is 1, then reg0xF = 1, else 0
 */
void chip8::8xy6() {
    // get the reg number
    int x = (opcode & 0x0F00) >> 8;

    // update register0xF
    registers[0xF] = registers[x] & 0x01;

    // update reg x
    registers[x] >>= 1;
}

/*
 * OPcode 8xy7
 * reg x = reg y - reg x
 * if reg y > reg x, reg0xF = 1, else 0
 */
void chip8::8xy7() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // set register0xF's value
    registers[0xF] = registers[y] > registers[x] ? 1 : 0;

    // update regx
    registers[x] = registers[y] - registers[x];
}

/*
 * OPcode 8xyE
 * left shift reg x's value by 1 (multiply by 2)
 * if most significant bit of reg x is 1, then reg0xF = 1, else 0
 */
void chip8::8xyE() {
    // get the reg number
    int x = (opcode & 0x0F00) >> 8;

    // update register0xF
    registers[0xF] = (registers[x] & 0x80) >> 7;

    // update reg x
    registers[x] <<= 1;
}

/*
 * OPcode 9xy0
 * skip the next instruction if reg x is not equal to reg y
 */
void chip8::9xy0() {
    // get the reg numbers
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    // check if regx != regy
    if (registers[x] != registers[y]) {
        // update pc to skip next instruction
        pc += 2;
    }
}

/*
 * OPcode Annn
 * load the address nnn into the index register
 */
void chip8::Annn() {
    index = opcode & 0x0FFF;
}

/*
 * OPcode Bnnn
 * jump to location nnn + value in reg 0
 */
void chip8::Bnnn() {
    // update pc to jump
    //uint16_t adr = registers[0] + (opcode & 0x0FFF);
    pc = registers[0] + (opcode & 0x0FFF);
}

/*
 * OPcode Cxkk
 * load reg x with the a random value ANDed with kk
 */
void chip8::Cxkk() {
    // get the reg number
    int x = (opcode & 0x0F00) >> 8;

    // generate a random number between 0 and 255 inclusive
    srand(time(NULL));
    uint8_t rnd = rand() % 256;

    // get the vlaue kk from the opcode
    uint8_t val = opcode & 0x00FF;

    // update reg x
    registers[x] = rnd & val;
}

/*
 * OPcode Dxyn
 * 
 */
void chip8::Dxyn();
void chip8::Ex9E();
void chip8::ExA1();
void chip8::Fx07();
void chip8::Fx0A();
void chip8::Fx15();
void chip8::Fx18();
void chip8::Fx1E();
void chip8::Fx29();
void chip8::Fx33();
void chip8::Fx55();
void chip8::Fx65();