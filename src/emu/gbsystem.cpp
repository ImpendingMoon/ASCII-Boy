/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbsystem.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Contains and coordinates all the component classes of the emulated Gameboy
 ******************************************************************************/

#include "gbsystem.hpp"

// Constructor
GBSystem::GBSystem(const std::string& rom_path)
{
	internal_speed = 4194304; // GB always starts out in standard speed mode

	// Check if ROM file path exists and is accessible
	if(!std::filesystem::exists(rom_path))
	{
		throw new std::invalid_argument("File not found.");
	}

	rom_file_path = rom_path;

	cart = std::make_unique<Cartridge>(rom_file_path, mem);
}

// Destructor
GBSystem::~GBSystem() = default;



// Steps the system by one CPU instruction;
void GBSystem::step()
{
	// Fetch
	uint8_t opcode = mem.readByte(cpu.getShortReg(gbstructs::PC));

	// Decode/Execute
	int cycles = cpu.execute(opcode, mem);

	// Step other components
	for(int i = 0; i < cycles; i++)
	{
		// In terms of other components, we have no other components.
	}
}