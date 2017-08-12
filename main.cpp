#include <stdio.h>
#include <stdlib.h>

#include "nes_file.h"
#include "debug.h"

int main(int argc, char **argv) {
	Debug::initDebug();
	printf("works\n");
	FILE *fp = NULL; 
	fp = fopen("roms/NES Test.nes", "r");
	if (!fp) {
		fprintf(stderr, "Failed to open file\n");
		exit(EXIT_FAILURE);
	}
	readRom(fp);
	printRom(fp);
	return EXIT_SUCCESS;
}
