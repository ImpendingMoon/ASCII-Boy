/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cart.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 9 Dec 2022
 EDITED : 9 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles loading the ROM, verifying its integrity, and loading it into an MMU
 ******************************************************************************/


#pragma once

#include "../core.hpp"
#include "gbstructs.hpp"
#include "mmu.hpp" // Cartridge loads directly into an MMU

class Cartridge
{
public:
	// Constructor
	Cartridge(std::string rom_file_path, MMU* mem);
	// Destructor
	virtual ~Cartridge();

	// Gets the ROM file path
	std::string getROMFilePath();
	// Gets the SAV file path
	std::string getSAVFilePath();

	// Gets the number of ROM Banks
	int getROMBankAmount();
	// Gets the number of RAM Banks
	int getRAMBankAmount();

private:

	std::string rom_file_path;
	std::string sav_file_path;

	std::ifstream RomFile;

	std::string game_title; // The game's title is store inside the header.
	int rom_bank_amount;
	int ram_bank_amount;
	int mbc_id;

	// Loads the RomFile from the rom_file_path
	void loadROM(MMU* mem);
	// Returns if the ROM header of the RomFile is valid.
	static bool isHeaderValid(std::array<uint8_t, 80>& header);
	// Gets the game title from the ROM header
	static std::string parseGameTitle(std::array<uint8_t, 80>& header);
	// Creates the sav_file_path from the rom_file_path
	std::string createSAVFilePath();
};