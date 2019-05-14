#ifndef PPU_H
#define PPU_H

#include "utils.h"

namespace ppu {

constexpr u16 CONTROL_REG_1 = 0x2000;
constexpr u16 CONTROL_REG_2 = 0x2001;
constexpr u16 PPU_STATUS_REG = 0x2002;

enum class ControlReg1Flags : u8 {
    NAME_TABLE_0 = 0,
    NAME_TABLE_1 = 1,
    NAME_TABLE_2 = 2,
    NAME_TABLE_3 = 3,
    INCREMENT_32 = 4, // if this bit is set in reg1, address should be
                      // incremented by 32, otherwise increment by 1
    PATTERN_TABLE = 8, // if set, sprites are stored in pattern trable 1,
                       // 0 otherwise
    BACKGROUND_TABLE = 16, // same as pattern table
    SPRITE_8_16 = 32, // if set sprites are 8x16, otherwise 8x8
    NMI_ENABLED = 128
};

enum class ControlReg2Flags : u8 {
    MONOCHROME_MODE = 1, // if set, system is in monochrome mode, color mode otherwise
    CLIP_MODE = 2,      // if set, the background in the left 8 pixels will
                        // be shown, otherwise they will be hidden
    SPRITE_CLIP_MODE = 4, // if set, sprites wil be shown in the left 8 pixels
    DISPLAY_BACKGROUND = 8,
    DISPLAY_SPRITES = 16,
    BACKGROUND1 = 32,
    BACKGROUND2 = 64,
    BACKGROUND3 = 128
};

enum class PPUStatusFlags : u8 {
    IGNORE_VRAM = 16, // when set vram should be ignored
    SCANLINE_SPRITE_COUNT = 32, // if set, more than 8 sprites are on the
                                // current scanline
    SPRITE_0_HIT_FLAG = 64, // set when a non-transperent pixel of sprite 0
                            // overlaps with a non-transperent background pixel
    V_BLANK_OCCURING = 128 // set when a V-Blank is occuring
};

constexpr u16 SPRITE_MEMORY_ADDRESS_REG = 0x2003;
constexpr u16 SPRITE_MEMORY_DATA_REG = 0x2004;
constexpr u16 SCREEN_SCROLL_OFFSETS_REG = 0x2005;

constexpr u16 IO_REG_1 = 0x2006;
constexpr u16 IO_REG_2 = 0x2007;
constexpr u16 DMA_REGISTER = 0x4014;



constexpr u16 PATTERN_TABLE_0 = 0x0000;
constexpr u16 PATTERN_TABLE_1 = 0x1000;
constexpr u16 NAME_TABLE_0 = 0x2000;
constexpr u16 NAME_TABLE_1 = 0x2400;
constexpr u16 NAME_TABLE_2 = 0x2800;
constexpr u16 NAME_TABLE_3 = 0x2C00;
constexpr u16 ATTRIB_TABLE_0 = 0x23C0;
constexpr u16 ATTRIB_TABLE_1 = 0x27C0;
constexpr u16 ATTRIB_TABLE_2 = 0x2BC0;
constexpr u16 ATTRIB_TABLE_3 = 0x2FC0;
constexpr u16 IMAGE_PALETTE = 0x3F00;
constexpr u16 SPRITE_PALETTE = 0x3F10;

constexpr u32 PPU_MIRROR_START = 0x4000;
constexpr u32 PPU_MIRROR_END = 0x10000;

void tick(u8 count);

}


#endif
