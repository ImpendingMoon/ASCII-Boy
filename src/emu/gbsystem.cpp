/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbsystem.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Contains and coordinates all the component classes of the emulated Gameboy
 ******************************************************************************/

#include "gbsystem.hpp"

using namespace std;

// Constructor
GBSystem::GBSystem(string rom_file_path)
{

	internal_speed = 4194304; // GB always starts out in standard speed mode

	// Check if ROM file path exists and is accessable
	if(!filesystem::exists(rom_file_path))
	{
		throw new invalid_argument("File not found.");
	}

	this->rom_file_path = rom_file_path;

}

// Destructor
GBSystem::~GBSystem()
{
}
