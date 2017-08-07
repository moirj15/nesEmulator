#ifndef SFOT_CPU
#define SFOT_CPU

struct Registers {
	unsigned short pc;
	unsigned char sp;
	char accumulator;
	char x;
	char y;
	unsigned char flags;
};



#endif
