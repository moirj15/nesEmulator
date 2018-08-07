#include <cstdio>
#include <cstdlib>
#include <vector>
#include "cpu.h"
#include "debugger.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
    std::vector<u8> code;
    code = {
        0xa2, 0x08, 0xca, 0x8e, 0x00, 0x02, 0xe0, 
        0x03, 0xd0, 0x02, 0x8e, 0x01, 0x02, 
	};
	
    Cpu::init();

    

    //printf("%s\n", dissassemble_inst(0xa2).c_str());

	Cpu::testCpu(code);
	return EXIT_SUCCESS;
}
