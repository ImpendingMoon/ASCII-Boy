/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbsystem.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 8 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Contains and coordinates all the component classes of the emulated Gameboy
 ******************************************************************************/

#include "gbsystem.hpp"

// Constructor
GBSystem::GBSystem(std::string rom_file_path)
{
	internal_speed = 4194304; // GB always starts out in standard speed mode

	// Check if ROM file path exists and is accessable
	if(!std::filesystem::exists(rom_file_path))
	{
		throw new std::invalid_argument("File not found.");
	}

	this->rom_file_path = rom_file_path;

	cart = new Cartridge(rom_file_path, mem);
}

// Destructor
GBSystem::~GBSystem()
{
	delete cart;
}
