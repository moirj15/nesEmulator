#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "opCodes.h"


const string initFile = "init/initFile.txt";

Opcode::~Opcode() {

}

/**
 * Opens the init file and initializes the vector of op-codes.
 * 		will be in the format:
 * 		inst address-mode cycles\n
 */
void initDebugOpCodes() {
	FILE *init = NULL; 
	init = fopen(initFile.c_str(), "r");
	if (!init) {
		fprintf(stderr, "FAILED TO READ INIT FILE\n");
		exit(EXIT_FAILURE);
	}

	DebugOpCodes = vector<Opcode>();
	DebugOpCodes.reserve(256);

	while (!feof(init)) {
		uint8_t am, inst, cyc;
		fscanf(init, "%hhu %hhu %hhu\n", &inst, &am, &cyc);
		DebugOpCodes[inst] = Opcode(am, inst, cyc);
	}
}
