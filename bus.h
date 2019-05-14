#ifndef BUS_H
#define BUS_H

#include "utils.h"

namespace bus {

constexpr u16 RAM_SECTION_END = 0x2000;
// size of non-mirrored ram
constexpr u16 RAM_SIZE = 0x0800;
// use to get the correct address if accessing the mirrored ram
constexpr u16 RAM_MASK = 0x07FF;

constexpr u16 LOW_IO_REGISTERS_START = 0x2000;
constexpr u16 LOW_IO_REGISTERS_END = 0x2000;
constexpr u16 LOW_IO_REG_MASK = 0x07;
constexpr u16 LOW_IO_REG_COUNT = 8;
constexpr u16 HIGH_IO_REG_COUNT = 0x20;
constexpr u16 HIGH_IO_REGISTERS_END = 0x4020;
constexpr u16 EXPANSION_ROM = 0x4020;
constexpr u16 SRAM = 0x6000;
constexpr u16 PRG_ROM_LOWER_BANK = 0x8000;
constexpr u16 PRG_ROM_UPPER_BANK = 0xC000;

constexpr u16 SRAM_SIZE = PRG_ROM_LOWER_BANK - SRAM;
constexpr u16 PRG_ROM_LOWER_BANK_SIZE = PRG_ROM_UPPER_BANK - PRG_ROM_LOWER_BANK;
constexpr u16 PRG_ROM_UPPER_BANK_SIZE = 0x10000 - PRG_ROM_UPPER_BANK;

struct CpuBus {
    u8 ram[RAM_SIZE];
    u8 low_io_regs[LOW_IO_REG_COUNT];
    u8 high_io_regs[HIGH_IO_REG_COUNT];
    u8 sram[SRAM_SIZE];
    u8 prg_rom_lower_bank[PRG_ROM_LOWER_BANK_SIZE];
    u8 prg_rom_upper_bank[PRG_ROM_UPPER_BANK_SIZE];

    u8 read(u16 addr);
    void write(u16 addr, u8 val);
};

}


#endif
