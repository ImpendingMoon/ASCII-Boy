/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbsystem.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 3 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Contains and coordinates all the component classes of the emulated Gameboy
 ******************************************************************************/

#pragma once

#include "../core.hpp"

class GBSystem
{
public:
	// Constructor
	GBSystem();
private:
	std::string rom_file_path;
	std::string sav_file_path;

	// TODO: Add Components as added
};
