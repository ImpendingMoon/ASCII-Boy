/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cpu.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main CPU of the Gameboy. Handles registers, opcode execution, and cycles.
 ******************************************************************************/

#include "cpu.hpp"

CPU::CPU()
{
	// Set initial register values
	// Equivalent to DMG0 values
	regs.a = 0x01;
	regs.f = 0x00;
	regs.b = 0xFF;
	regs.c = 0x13;
	regs.d = 0x00;
	regs.e = 0xC1;
	regs.h = 0x84;
	regs.l = 0x93;
	regs.sp = 0x0100;
	regs.pc = 0xFFFE;

	halted = false;
	next_halted_state = false;
}
