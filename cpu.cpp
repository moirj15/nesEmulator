#include "cpu.h"

// Add with carry instructions
#define ADC_IMM		0x69
#define ADC_ZP		0x65
#define ADC_ZPX		0x75
#define ADC_ABS		0x6D
#define ADC_ABS_X	0x7D
#define ADC_ABS_Y	0x79
#define ADC_IND_X	0x61
#define ADC_IND_Y	0x71

#define NOP		0xEA

Cpu::Cpu(void)
{
	A = 0;
	X = 0;
	Y = 0;
	//TODO: set SP and PC
	status = 0;
	remainingCycles = CYCLES_PER_FRAME;
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

void Cpu::run(void)
{
	switch (memory[PC]) {
		case NOP:
			nop();
			break;
	}
}

/**
 * Add with carry instructions.
 */
void Cpu::addc_imm(void)
{
	pc++;
	u8 preAdd = A >> 7;
	A += memory[pc] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;
	tick();
	tick();
}

void Cpu::addc_zp(void)
{
	addc_imm();
	tick();
}

void Cpu::addc_zpx(void)
{
	pc++;
	u8 preAdd = A >> 7;
	A += memory[memory[pc] + X] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;

	tick();
	tick();
	tick();
	tick();

}

void Cpu::addc_abs(void)
{
	pc++;
	u8 preAdd = A >> 7;
	u16 memLoc = memory[pc] << 8;
	pc++;
	memLoc |= memory[pc];
	A += memory[memLoc] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;
	tick();
	tick();
	tick();
	tick();
}

void Cpu::addc_abs_x(void)
{
	pc++;
	u8 preAdd = A >> 7;
	u16 memLoc = memory[pc] << 8;
	pc++;
	memLoc |= memory[pc];
	A += memory[memLoc + X] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;
	tick();
	tick();
	tick();
	tick();
	//TODO: check if page is crossed

}

void Cpu::addc_abs_y(void)
{
	pc++;
	u8 preAdd = A >> 7;
	u16 memLoc = memory[pc] << 8;
	pc++;
	memLoc |= memory[pc];
	A += memory[memLoc + Y] + (CARRY & status);
	u8 postAdd = A >> 7;
	status |= (preAdd != postAdd);
	status |= (A == 0) << 1;
	status |= (A & NEGATIVE) << 7;
	pc++;
	tick();
	tick();
	tick();
	tick();
	//TODO: check if page is crossed
}

void Cpu::addc_ind_x(void)
{

}

void Cpu::addc_ind_y(void)
{

}
	
/**
 * No operation.
 */
void Cpu::nop(void)
{
	pc++;
	tick();
	tick();
}
