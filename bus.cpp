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

}

void CpuBus::write(u16 addr, u8 val) {

}

}
