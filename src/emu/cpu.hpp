/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cpu.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main CPU of the Gameboy. Handles registers, opcode execution, and cycles.
 ******************************************************************************/

#pragma once

#include "../core.hpp"
#include "gbstructs.hpp"

using namespace gbstructs;

class CPU
{
public:
	CPU();
private:
	RegisterSet regs;
	FlagRegister flags;

	bool halted;
	bool next_halted_state;
};
