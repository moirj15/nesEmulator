#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cpu.h"
#include "debugger.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

    Cpu::test_immediate();
    Cpu::test_zero_page();
    Cpu::test_zero_page_X();
    Cpu::test_zero_page_Y();
    Cpu::test_absolute();
    Cpu::test_absolute_X();
    Cpu::test_absolute_Y();
    Cpu::test_indexed_indirect();
    Cpu::test_indirect_indexed();

    

    //printf("%s\n", dissassemble_inst(0xa2).c_str());

    //Cpu::testCpu(code);
	return EXIT_SUCCESS;
}
