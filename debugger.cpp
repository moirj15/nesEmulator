#include "debugger.h"

#include <map>
#include <string>

static std::map<u8, std::string> lookup;
static bool initialized = false;

static void init_lookup(void)
{
    
}

std::string dissassemble_inst(u8 instruction)
{
    if (!initialized) {
        init_lookup();
    }

    return lookup[instruction];
}
