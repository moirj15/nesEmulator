#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "utils.h"

enum ADDRESS_MODES : u8 {
    IMPLIED,
    IMMEDIATE,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT,
    INDEXED_INDIRECT, // (Indirect, X)
    INDIRECT_INDEXED, // (Indirect), Y
    ACCUMULATOR,
    RELATIVE,
    NA
};

struct OpCode {
    char name[6];
    u8 op;
    u8 bytes;
    u8 address_mode;
    u8 cycle_count;
};

/**
 * Decodes the given opcode into an OpCode struct.
 * @param op: The opcode.
 * @return: The OpCode object.
 */
OpCode decode_opcode(u8 op);


#endif
