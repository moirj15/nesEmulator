#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <vector>

using namespace std;

class Cpu {
private:
	vector<uint8_t> memory;
	uint8_t			accumulator;
	uint8_t			X;
	uint8_t			Y;
	uint8_t			flag;

public:
	Cpu();
	~Cpu();
};

#endif 
