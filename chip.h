#include <cstdint>

// macros for the class used below
#define NUM_REG         16
#define MEM_SIZE        4096
#define STACK_SIZE      16
#define KEYPAD_SIZE     16
#define VIDMEM_SIZE     2048    // 64*32
#define FONTSET_SIZE    80

/* 
 * class definition for the chip8 infrastructure 
 * the names are self explanatory and represent 
 * exactly what they say
 */
class chip8 {
    public:
        uint8_t registers[NUM_REG]{};
        uint8_t memory[MEM_SIZE]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[STACK_SIZE]{};
        uint8_t sp{};
        uint8_t delaytimer{};
        uint8_t soundtimer{};
        uint8_t keypad[KEYPAD_SIZE]{};
        uint32_t video[VIDMEM_SIZE]{};
        uint16_t opcode;

        chip8();
        void loadROM(char const* filename);

};

/* 
 * the fonntset that represents the characters on the 
 * right in the comments
 * the representation in of a pixel art of the size 4 x 5
 * example: 0 is:
 *      11110000
 *      10010000
 *      10010000
 *      10010000
 *      11110000
 */
uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
    0x20, 0x60, 0x20, 0x20, 0x70,   // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
    0x9-, 0x90, 0xF0, 0x10, 0x10,   // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
    0xF0, 0x80, 0xF0, 0x90, 0xf0,   // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
    oxF0, 0x80, 0x80, 0x80, 0xF0,   // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
    0xF0, 0x80, 0xF0, 0x80, 0x80    // F
};