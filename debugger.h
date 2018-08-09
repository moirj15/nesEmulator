#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "utils.h"
#include <vector>
#include <string>

class Debugger
{
    enum Commands 
    {
        STEP,
        RUN,
        QUIT,
    };
    std::vector<u8>     instructions;
    std::vector<u16>    break_points;
    static std::string  lookup[0xff];
    static bool         initialized;


public:
    Debugger(void) {} 
    Debugger(const std::vector<u8> &inst) : instructions(inst) {}
    ~Debugger(void) {}

    void do_command(u32 command);
private:
    /**
     * Initializes the instruction lookup table.
     */
    static void init_lookup(void);
    /**
     * Dissasembles the given instruction.
     *
     * @param instruction: The machine code that will be dissassembled.
     * @return: The dissassembled instruction.
     */
    std::string dissassemble_inst(u8 instruction);

    std::string print_db_info(void);



};


#endif
