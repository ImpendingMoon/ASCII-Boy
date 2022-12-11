/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cpu.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main CPU of the Gameboy. Handles registers, opcode execution, and cycles.
 ******************************************************************************/

#pragma once

#include "../core.hpp"
#include "gbstructs.hpp"
#include "mmu.hpp"

using namespace gbstructs;

class CPU
{
public:
	CPU();

	// Executes an opcode, returns the number of cycles used
	int execute(uint8_t opcode, MMU& mem);

	// SGetters

	// Gets a byte from an 8-bit register
	uint8_t getByteReg(TargetID target);
	// Sets an 8-bit register to a value
	void setByteReg(TargetID target, uint8_t value);
	// Gets a short from a 16-bit register
	uint16_t getShortReg(TargetID target);
	// Sets a 16-bit register to a value
	void setShortReg(TargetID target, uint16_t value);

private:
	RegisterSet regs{};
	FlagRegister flags{};

	bool halted;
	bool next_halted_state;

	// Converts a 3-bit ID to a TargetID
	TargetID toTarget(uint8_t id);
};
