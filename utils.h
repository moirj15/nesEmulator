#ifndef UTILS_H
#define  UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define local static
#define db() printf("hit\n");

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;


typedef int8_t      s8;
typedef int16_t     s16;
typedef int32_t     s32;
typedef int64_t     s64;


/**
 * Exits the program with EXIT_FAILURE and prints the given 
 * error message.
 *
 * @param qMessage: The message to print to stderr.
 */
void quit(const char *qMessage);

/**
 * Opens the given file and handles any errors that may occur.
 * On failure it will print an error message to stderr.
 *
 * @param dir: The directory of the file.
 * @param permission: The permissions to open the file with.
 * @return: The opened file.
 */
FILE *openFile(const char *dir, const char *permission);

/**
 * Closes the given file and sets the pointer to NULL.
 *
 * @param fp: The file to close.
 */
void closeFile(FILE *fp);

#endif // UTILS_H
