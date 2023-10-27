#include "base/logger.h"
#include "main.h"

using Byte = unsigned char;
using Word = unsigned short;

using uint = unsigned int;

struct Mem {
private:
    Logger::Logger logger;
public:
    static const uint MAX_MEMORY = 1024 * 64;
    Byte Data[MAX_MEMORY];

    Mem(Logger::Logger l) : logger(l) {};

    void init() {
        for (int i = 0; i < MAX_MEMORY; i++) {
            Data[i] = 0;
        }
    }

    Byte operator[](uint addr) const { // this is run when getting the value
        return Data[addr];
    }

    Byte &operator[](uint addr) { // this is run when setting it equal to something (memory[addr] = idfk;)
        return Data[addr];
    }

    void WWord(uint& cycles, uint addr, Word word) { // write a word to the memory, 2 bytes
        Data[addr] = word & 0xFF;
        Data[addr + 1] = (word >> 8);
        logger.info("wrote word to " + std::to_string(addr));
        cycles -= 2;
    }
};

class CPU {
    Logger::Logger logger;

    void LDA_handle_flags() {
        Z = (A == 0) ? 1 : 0;
        N = ((A & 0b10000000) > 0) ? 1 : 0;
    }
public:
    Word PC;
    Word SP;

    Byte A, X, Y; // registers, accumulator, x reg, y reg

    // status flags

    Byte C : 1; // carry flag
    Byte Z : 1; // zero flag
    Byte I : 1; // interrupt disable
    Byte D : 1; // decimal mode
    Byte B : 1; // break command
    Byte V : 1; // overflow
    Byte N : 1; // negative flag

    CPU(Logger::Logger l) : logger(l) {};

    void Reset(Mem &memory) {
        PC = 0xFFFC;
        SP = 0x0100; // hex for 256
        D = 0;
        A = X = Y = 0;
        C = Z = I = D = B = V = N = 0;
        memory.init();
    }

    Byte FByte(Mem& memory, uint& cycles) { // fetch a byte from memory
        if (DEBUG) {logger.debug("Fetched: " + std::to_string(PC));}
        Byte data = memory[PC];
        logger.info("fetched byte " + std::to_string(data) + " from " + std::to_string(PC));
        PC++;
        cycles--;
        return data;
    }

    Word FWord(Mem& memory, uint& cycles) { // fetch a word from memory
        Word data = memory[PC];
        logger.info("fetched word " + std::to_string(data) + " from " + std::to_string(PC));
        PC++;
        data |= (memory[PC] << 8); // bit shift it by 8 places, |= is bitwise OR
        PC++;
        cycles -= 2;
        return data;
        // if your platform were big-endian you would have to swap the bits in the word
    }

    Byte RByte (Mem& memory, uint& cycles, Byte addr) { // read a byte from memory, difference being the program counter is not incremented
        if (DEBUG) {logger.debug("Read: " + std::to_string(PC));}
        Byte data = memory[addr];
        logger.info("read byte " + std::to_string(data) + " from " + std::to_string(PC));
        cycles--;
        return data;
    }

    // op codes
    static const Byte INS_LDA_IM = 0xA9, // instruction load accumulator, addressing mode: immediate
    INS_LDA_ZP = 0xA5, // instruction load accumulator, addressing mode: zero page
    INS_LDA_ZPX = 0xB5, // instruction load accumulator, addressing mode: zero page + X reg
    INS_JSR_ABS = 0x20; // instruction jump to subroutine

    void Execute(Mem& memory, uint cycles) {
        while ( cycles > 0) {
            Byte instruction = FByte(memory, cycles);

            switch (instruction) {

                case INS_LDA_IM: {
                    if (DEBUG) logger.debug("ran instruction LDA IMMEDIATE MODE");
                    Byte value = FByte(memory, cycles);
                    A = value;
                    LDA_handle_flags(); // checks if bit 7 (the eight bit) is 1 therefore making the number negative (twos comp)

                    break;
                }

                case INS_LDA_ZP: {
                    Byte zero_page_addr = FByte(memory, cycles);

                    Byte value = RByte(memory, cycles, zero_page_addr);
                    A = value;
                    LDA_handle_flags();

                    break;
                }

                case INS_LDA_ZPX: {
                    Byte zero_page_addr = FByte(memory, cycles);

                    Byte value = RByte(memory, cycles, zero_page_addr + X);
                    A = value;
                    LDA_handle_flags();

                    break;
                }

                case INS_JSR_ABS: {
                    // get the address of the subroutine from memory
                    // write the address of the return point of the subroutine - 1
                    // jump the program counter to the start of the subroutine
                    Word subroutine_addr = FWord(memory, cycles);
                    memory.WWord(cycles, SP, PC - 1);
                    SP++;
                    PC = subroutine_addr;
                    cycles--;
                    break;
                }

                default:
                    logger.critical("Instruction not handled, " + std::to_string(instruction));
                    break;
            }
        }
    }
};

int main() {
    Logger::Logger logger("6502");
    Logger::Logger memory_logger("MEMORY");
    Mem memory(memory_logger);
    CPU cpu(logger);
    cpu.Reset(memory);

    // hard coded 'program'
    //memory[0xFFFC] = CPU::INS_LDA_IM;
    //memory[0xFFFD] = 0x42;

    //memory[0xFFFC] = CPU::INS_LDA_ZP;
    //memory[0xFFFD] = 0x42;
    //memory[0x42] = 0x69;

    memory[0xFFFC] = CPU::INS_JSR_ABS;
    memory[0xFFFD] = 0x42;
    memory[0xFFFE] = 0x42;

    cpu.Execute(memory, 6);
    return 0;
}
