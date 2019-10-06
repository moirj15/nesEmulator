#include "bus.h"

namespace bus {

u8 CpuBus::read(u16 addr) {
    if (addr < RAM_SECTION_END) {
        return ram[addr & RAM_MASK];
    }
    else if (addr < LOW_IO_REGISTERS_END) {
        return low_io_regs[LOW_IO_REGISTERS_END - addr];
    }
    else if (addr < HIGH_IO_REGISTERS_END) {
        return high_io_regs[HIGH_IO_REG_COUNT - addr];
    }
    else if (addr < SRAM) {
        return sram[SRAM - addr];
    }
    else if (addr < PRG_ROM_LOWER_BANK) {
        return prg_rom_lower_bank[PRG_ROM_LOWER_BANK - addr];
    }
    else {
        return prg_rom_upper_bank[PRG_ROM_UPPER_BANK - addr];
    }
}

void CpuBus::write(u16 addr, u8 val) {
    if (addr < RAM_SECTION_END) {
        ram[addr & RAM_MASK] = val;
    }
    else if (addr < LOW_IO_REGISTERS_END) {
        low_io_regs[LOW_IO_REGISTERS_END - addr] = val;
    }
    else if (addr < HIGH_IO_REGISTERS_END) {
        high_io_regs[HIGH_IO_REG_COUNT - addr] = val;
    }
    else if (addr < SRAM) {
        sram[SRAM - addr] = val;
    }
    else if (addr < PRG_ROM_LOWER_BANK) {
        prg_rom_lower_bank[PRG_ROM_LOWER_BANK - addr] = val;
    }
    else {
        prg_rom_upper_bank[PRG_ROM_UPPER_BANK - addr] = val;
    }
}

}
