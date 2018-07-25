#include <cstdio>
#include <cstdlib>
#include <vector>
#include "cpu.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

    std::vector<u8> code;
    code = {
        0xa9, 0x01, 0x8d, 0x00, 0x02, 0xa9, 0x05, 0x8d, 0x01, 0x02, 
        0xa9, 0x08, 0x8d, 0x02, 0x02
	};
	
	Cpu::testCpu(code);
	
	return EXIT_SUCCESS;
}
