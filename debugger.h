#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "utils.h"
#include <vector>
#include <string>

class Debugger
{
    std::vector<u8>     instructions;
    std::vector<u16>    break_points;
    static std::string  lookup[0xff];

public:
    Debugger(void);
    Debugger(const std::vector<u8> &inst);
    ~Debugger(void);

private:
    std::string dissassemble_inst(u8 instruction);
    static void init_lookup(void);

};


#endif
