#include "ppu.h"

namespace ppu {

void tick(u8 count) {
    // ppu's clock ticks 3 times for every cpu clock tick
    for (u32 i = 0; i < count * 3; i++) {

    }
}

}
