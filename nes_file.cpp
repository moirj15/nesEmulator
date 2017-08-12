#include "nes_file.h"
#include <stdlib.h>
#include <string.h>

static unsigned char *getHeader(FILE *rom);
static bool checkHeader(unsigned char *header);
static unsigned char getMapper(unsigned char lowBits, unsigned char highBit);

NesFile *readRom(FILE *rom) {
	NesFile *nes = new NesFile();

	unsigned char *header = getHeader(rom);

	nes->romBanks = header[4];
	nes->vRomBanks = header[5];
	nes->mirroring = header[6] & 1;
	nes->battery = (header[6] & 2) >> 1;
	nes->trainerPresent = (header[6] & 4) >> 2;
	nes->fourScreenVRam = (header[6] & 8) >> 3;
	nes->mapperType = getMapper(header[6], header[7]);	
	nes->ramBanks =  
	
	return nes;
}

void printRom(FILE *file) {
	unsigned char byte;
	int line = 0;
	while (!feof(file)) {
		if (line % 8 == 0) {
			printf("\n");
			line = 0;
		}
		fscanf(file, "%c", &byte);
		printf("%02x", byte);
		line++;
	}
}

static unsigned char *getHeader(FILE *rom) {
	unsigned char *header = new unsigned char[16];
	for (int i = 0; i < 16; i++) {
		fscanf(rom, "%c", &(header[i]));
	}
	/*if (!checkHeader(header)) {
		fprintf(stderr, "Invalid .NES file\n");
		exit(EXIT_FAILURE);
	}*/
	return header;
}


static bool checkHeader(unsigned char *header) {
	return strncmp((char *)header, "NES", 4);
}


static unsigned char getMapper(unsigned char lowBits, unsigned char highBit) {
	unsigned char byte;
	byte = 0x0F & highBit;
	byte |= (0x0F & lowBits) >> 4;
	return byte;
}












