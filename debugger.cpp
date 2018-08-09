#include "debugger.h"
#include "cpu.h"

#include <string>

std::string Debugger::lookup[0xff] = {""};
bool Debugger::initialized = false;

void Debugger::do_command(u32 command)
{
    switch(command) {
    case STEP:
        Cpu::step();
        print_db_info();
        break;
    case RUN:
        break;
    case QUIT:
        break;

    default:
        return;
    }

}

void Debugger::init_lookup(void)
{
    lookup[0xA2] = "LDX_IMM"	;	
    lookup[0xA6] = "LDX_ZP"		;
    lookup[0xB6] = "LDX_ZP_Y"	;
    lookup[0xAE] = "LDX_ABS"	;	
    lookup[0xBE] = "LDX_ABS_Y"	;
    lookup[0xA0] = "LDY_IMM"	;	
    lookup[0xA4] = "LDY_ZP"		;
    lookup[0xB4] = "LDY_ZP_X"	;
    lookup[0xAC] = "LDY_ABS"	;	
    lookup[0xBC] = "LDY_ABS_X"	;
    lookup[0x85] = "STA_ZP"		;
    lookup[0x95] = "STA_ZP_X"	;
    lookup[0x8D] = "STA_ABS"	;	
    lookup[0x9D] = "STA_ABS_X"	;
    lookup[0x99] = "STA_ABS_Y"	;
    lookup[0x81] = "STA_IND_X"	;
    lookup[0x91] = "STA_IND_Y"	;
    lookup[0x86] = "STX_ZP"		;
    lookup[0x96] = "STX_ZP_Y"	;
    lookup[0x8E] = "STX_ABS"	;	
    lookup[0x84] = "STY_ZP"		;
    lookup[0x94] = "STY_ZP_X"	;
    lookup[0x8C] = "STY_ABS"	;	
    lookup[0xAA] = "TAX"		;	
    lookup[0xA8] = "TAY"		;	
    lookup[0x8A] = "TXA"		;	
    lookup[0x98] = "TYA"		;	
    lookup[0xBA] = "TSX"		;	
    lookup[0x9A] = "TXS"		;	
    lookup[0x48] = "PHA"		;	
    lookup[0x08] = "PHP"		;	
    lookup[0x68] = "PLA"		;	
    lookup[0x28] = "PLP"		;	
    lookup[0x29] = "AND_IMM"	;	
    lookup[0x25] = "AND_ZP"		;
    lookup[0x35] = "AND_ZP_X"	;
    lookup[0x2D] = "AND_ABS"	;	
    lookup[0x3D] = "AND_ABS_X"	;
    lookup[0x39] = "AND_ABS_Y"	;
    lookup[0x21] = "AND_IND_X"	;
    lookup[0x31] = "AND_IND_Y"	;
    lookup[0x49] = "EOR_IMM"	;	
    lookup[0x45] = "EOR_ZP"		;
    lookup[0x55] = "EOR_ZP_X"	;
    lookup[0x4D] = "EOR_ABS"	;	
    lookup[0x5D] = "EOR_ABS_X"	;
    lookup[0x59] = "EOR_ABS_Y"	;
    lookup[0x41] = "EOR_IND_X"	;
    lookup[0x51] = "EOR_IND_Y"	;
    lookup[0x09] = "ORA_IMM"	;	
    lookup[0x05] = "ORA_ZP"		;
    lookup[0x15] = "ORA_ZP_X"	;
    lookup[0x0D] = "ORA_ABS"	;	
    lookup[0x1D] = "ORA_ABS_X"	;
    lookup[0x19] = "ORA_ABS_Y"	;
    lookup[0x01] = "ORA_IND_X"	;
    lookup[0x11] = "ORA_IND_Y"	;
    lookup[0x24] = "BIT_ZP"		;
    lookup[0x2C] = "BIT_ABS"	;	
    lookup[0x69] = "ADC_IMM"	;	
    lookup[0x65] = "ADC_ZP"		;
    lookup[0x75] = "ADC_ZP_X"	;
    lookup[0x6D] = "ADC_ABS"	;	
    lookup[0x7D] = "ADC_ABS_X"	;
    lookup[0x79] = "ADC_ABS_Y"	;
    lookup[0x61] = "ADC_IND_X"	;
    lookup[0x71] = "ADC_IND_Y"	;
    lookup[0xE9] = "SBC_IMM"	;	
    lookup[0xE5] = "SBC_ZP"		;
    lookup[0xF5] = "SBC_ZP_X"	;
    lookup[0xED] = "SBC_ABS"	;	
    lookup[0xFD] = "SBC_ABS_X"	;
    lookup[0xF9] = "SBC_ABS_Y"	;
    lookup[0xE1] = "SBC_IND_X"	;
    lookup[0xF1] = "SBC_IND_Y"	;
    lookup[0xC9] = "CMP_IMM"	;	
    lookup[0xC5] = "CMP_ZP"		;
    lookup[0xD5] = "CMP_ZP_X"	;
    lookup[0xCD] = "CMP_ABS"	;	
    lookup[0xDD] = "CMP_ABS_X"	;
    lookup[0xD9] = "CMP_ABS_Y"	;
    lookup[0xC1] = "CMP_IND_X"	;
    lookup[0xD1] = "CMP_IND_Y"	;
    lookup[0xE0] = "CPX_IMM"	;	
    lookup[0xE4] = "CPX_ZP"		;
    lookup[0xEC] = "CPX_ABS"	;	
    lookup[0xC0] = "CPY_IMM"	;	
    lookup[0xC4] = "CPY_ZP"		;
    lookup[0xCC] = "CPY_ABS"	;	
    lookup[0xE6] = "INC_ZP"		;
    lookup[0xF6] = "INC_ZP_X"	;
    lookup[0xEE] = "INC_ABS"	;	
    lookup[0xFE] = "INC_ABS_X"	;
    lookup[0xE8] = "INX"		;	
    lookup[0xC8] = "INY"		;	
    lookup[0xC6] = "DEC_ZP"		;
    lookup[0xD6] = "DEC_ZP_X"	;
    lookup[0xCE] = "DEC_ABS"	;	
    lookup[0xDE] = "DEC_ABS_X"	;
    lookup[0xCA] = "DEX"		;	
    lookup[0x88] = "DEY"		;	
    lookup[0x0A] = "ASL_ACC"	;	
    lookup[0x06] = "ASL_ZP"		;
    lookup[0x16] = "ASL_ZP_X"	;
    lookup[0x0E] = "ASL_ABS"	;	
    lookup[0x1E] = "ASL_ABS_X"	;
    lookup[0x4A] = "LSR_ACC"	;	
    lookup[0x46] = "LSR_ZP"		;
    lookup[0x56] = "LSR_ZP_X"	;
    lookup[0x4E] = "LSR_ABS"	;	
    lookup[0x5E] = "LSR_ABS_X"	;
    lookup[0x2A] = "ROL_ACC"	;	
    lookup[0x26] = "ROL_ZP"		;
    lookup[0x36] = "ROL_ZP_X"	;
    lookup[0x2E] = "ROL_ABS"	;	
    lookup[0x3E] = "ROL_ABS_X"	;
    lookup[0x6A] = "ROR_ACC"	;	
    lookup[0x66] = "ROR_ZP"		;
    lookup[0x76] = "ROR_ZP_X"	;
    lookup[0x6E] = "ROR_ABS"	;	
    lookup[0x7E] = "ROR_ABS_X"	;
    lookup[0x4C] = "JMP_ABS"	;	
    lookup[0x6C] = "JMP_IND"	;	
    lookup[0x20] = "JSR"		;	
    lookup[0x60] = "RTS"		;	
    lookup[0x90] = "BCC"		;	
    lookup[0xB0] = "BCS"		;	
    lookup[0xF0] = "BEQ"		;	
    lookup[0x30] = "BMI"		;	
    lookup[0xD0] = "BNE"		;	
    lookup[0x10] = "BPL"		;	
    lookup[0x50] = "BVC"		;	
    lookup[0x70] = "BVS"		;	
    lookup[0x18] = "CLC"		;	
    lookup[0xD8] = "CLD"		;	
    lookup[0x58] = "CLI"		;	
    lookup[0xB8] = "CLV"		;	
    lookup[0x38] = "SEC"		;	
    lookup[0xF8] = "SED"		;	
    lookup[0x78] = "SEI"		;	
    lookup[0x00] = "BRK"		;	
    lookup[0xEA] = "NOP"		;	
    lookup[0x40] = "RTI"		;	
}

std::string Debugger::dissassemble_inst(u8 instruction)
{
    if (!initialized) {
        init_lookup();
    }

    return lookup[instruction];
}

std::string Debugger::print_db_info(void)
{
    std::string ret = "";

    return ret;
}
