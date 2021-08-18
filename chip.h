#include <cstdint>

// macros for the class used below
#define NUM_REG     16
#define MEM_SIZE    4096
#define STACK_SIZE  16
#define KEYPAD_SIZE 16
#define VIDMEM_SIZE 2048    // 64*32

/* class definition for the chip8 infrastructure 
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