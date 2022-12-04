/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbsystem.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 4 Dec 2022
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

	// File path handling //

	// Check if file path exists and is accessable
	if(!filesystem::exists(rom_file_path))
	{
		throw new invalid_argument("File not found.");
	}

	this->rom_file_path = rom_file_path;


	// Save file is at ROM path with a .sav extension unless specified
	sav_file_path = filesystem::absolute(rom_file_path);
	
	// If file has an extension, remove it
	size_t extension_pos = sav_file_path.find_last_of('.');
	if(extension_pos != string::npos)
	{
		sav_file_path = sav_file_path.substr(0, extension_pos);
	}

	sav_file_path.append(".sav");

	// End file path handling //

	

}



