#ifndef CPU_H
#define CPU_H

#include "utils.h"

struct flags 
{
	CARRY = 1,
	ZERO = 2,
	INTERRUPT_DISSABLE = 4,
	DECIMAL_MODE = 8,
	OVERFLOW = 64,
	NEGATIVE = 128,
};

class Cpu
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

public:
	Cpu(void);
	~Cpu(void);

	void run(void);
	
private:
	/**
 	 * Run the Cpu for a cycle.
 	 */
	void tick(void);

	/**
	 * Add with carry instructions.
	 */
	void addc_imm(void);
	void addc_zp(void);
	void addc_zpx(void);
	void addc_abs(void);
	void addc_abs_x(void);
	void addc_abs_y(void);
	void addc_ind_x(void);
	void addc_ind_y(void);

	/**
	 * No operation.
	 */
	void nop(void);

};


#endif
