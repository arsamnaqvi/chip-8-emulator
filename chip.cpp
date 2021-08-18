#include "chip.h"
#include <fstream>

/* macro to define the address pointed to by the 
 * program counter upon boot time
 */
#define START_ADDRESS   0x200

/* 
 * chip8 constructor
 * initializes the system
 * inputs: none
 * outputs: settings initialized like the program counter
 */
chip8::chip8() {
    // initialize the pc for the system
    pc = START_ADDRESS;
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