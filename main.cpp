#include <cstdio>
#include <cstdlib>

#include "cpu.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	u8 code[] = {
		0x69, 0x05, 0xEA,
	};
	u32 size = 3;
	
	Cpu::testCpu(code, size);
	
	return EXIT_SUCCESS;
}
