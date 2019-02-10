#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "cpu.h"
#include "debugger.h"
#include "tests.h"


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

    Tests::run();
    

    //printf("%s\n", dissassemble_inst(0xa2).c_str());

    //Cpu::testCpu(code);
	return EXIT_SUCCESS;
}
