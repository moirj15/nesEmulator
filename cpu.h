#ifndef CPU_H
#define CPU_H

#include "utils.h"

struct Cpu
{
	// General purpose registers
	u8 	A;
	u8 	X;
	u8 	Y;

	// Special purpose registers
	u8 	SP;
	u8 	status;
	u16 PC;

	u8 memory[0x10000]; // for now allocate the entire address space for the emulator

	const s32 CYCLES_PER_FRAME = 29834;
	s32 remainingCycles; //borrowed from github/AndreaOrru/LaiNES 

	Cpu(void);
	~Cpu(void);

	/**
 	 * Run the Cpu for a cycle.
 	 */
	void tick(void);
};


#endif
