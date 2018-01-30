#include "cpu.h"

Cpu::Cpu(void)
{
	A = 0;
	X = 0;
	Y = 0;
	//TODO: set SP and PC
	status = 0;
	remaningCycles = CYCLES_PER_FRAME;
}

Cpu::~Cpu(void)
{

}

/**
 * Run the Cpu for a cycle.
 */
void Cpu::tick(void)
{
	// TODO: call ppu three times since it runs at 3 x the 6502 clock speed
	// NOTE: the cpu runs at 1.79 MHz which comes down to roughly 29834 cycles
	// per frame
	remainingCycles--;
}
