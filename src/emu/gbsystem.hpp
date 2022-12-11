/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbsystem.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Contains and coordinates all the component classes of the emulated Gameboy
 ******************************************************************************/

#pragma once

#include "../core.hpp"

#include "cpu.hpp"
#include "mmu.hpp"
#include "cart.hpp"

class GBSystem
{
public:
	// Constructor
	GBSystem(const std::string& rom_file_path);
	// Destructor
	virtual ~GBSystem();

	CPU cpu;
	MMU mem;
	std::unique_ptr<Cartridge> cart;

	// Steps the system by one CPU instruction
	void step();

private:
	std::string rom_file_path; // Full file path for the GB ROM

	uint32_t internal_speed; // The processor speed in Hz

};
