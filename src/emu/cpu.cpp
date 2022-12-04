/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cpu.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 4 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main CPU of the Gameboy. Handles registers, opcode execution, and cycles.
 ******************************************************************************/

#include "cpu.hpp"

CPU::CPU()
{
	// Set initial register values
	// Regs set to equivalent of CGB in GB mode by default
	regs.a = 0x11;
	regs.f = 0b10000000;
	regs.b = 0x00;
	regs.c = 0x00;
	regs.d = 0x00;
	regs.e = 0x08;
	regs.h = 0x00;
	regs.l = 0x7C;
	regs.sp = 0xFFFE;
	regs.pc = 0x100;

	halted = false;
	next_halted_state = false;
}
