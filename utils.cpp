#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void quit(const char *qMessage) {
    fprintf(stderr, "%s\n", qMessage);
    exit(EXIT_FAILURE);
}

/**
* Opens the given file and handles any errors that may occur.
* On failure it will print an error message to stderr.
*
* @param dir: The directory of the file.
* @param permission: The permissions to open the file with.
* @return: The opened file.
*/
FILE *openFile(const char *dir, const char *permission) {
	FILE *fp = NULL;
	fp = fopen(dir, permission);
    if (!fp) {
        fprintf(stderr, "Failed to open file %s\n", dir);
        exit(EXIT_FAILURE);
    }
	return fp;
}

/**
* Closes the given file and sets the pointer to NULL.
*
* @param fp: The file to close.
*/
void closeFile(FILE *fp) {
    if (fclose(fp) < 0) {
        quit("Failed to close file");
    }
}
