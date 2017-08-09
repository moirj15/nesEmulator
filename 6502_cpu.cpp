#include "6502_cpu.h"

Registers::Registers(void) {
	pc = 0;
	sp = 0;
	accumulator = 0;
	x = 0;
	y = 0;
	flags = 0;
}

Cpu::Cpu(void) {
	reg = new Registers();
}

Cpu::~Cpu(void) {
	delete(reg);
}
