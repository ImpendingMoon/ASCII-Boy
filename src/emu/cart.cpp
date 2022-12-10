/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cart.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 9 Dec 2022
 EDITED : 9 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Handles loading the ROM, verifying its integrity, and loading it into an MMU
 ******************************************************************************/

#include "cart.hpp"

// This enum is mostly used for logging. It isn't even good at it. Too bad!
enum BankController
{
	// Equality given to match header value

	NONE = 0x00, // No additional banks.
	NONE_RAM = 0x08, // RAM
	NONE_BAT_RAM = 0x09, // Battery RAM

	MBC1 = 0x01, // Up to 2MiB ROM, 32KiB RAM
	MBC1_RAM = 0x02, // MBC1 with RAM
	MBC1_BAT_RAM = 0x03, // MBC1 with Battery RAM

	MBC2 = 0x05, // Up to 256KiB ROM, 256 bytes of mapped memory(?)
	MBC2_BAT = 0x06, //

	MBC3 = 0x11, // Up to 2MiB ROM, 32KiB RAM, Additional Timer
	MBC3_RAM = 0x12, // MBC3 with RAM
	MBC3_BAT_RAM = 0x13, // MBC3 with Battery RAM
	MBC3_BAT_TIMER = 0x0F, // MBC3 with Battery + Timer
	MBC3_BAT_RAM_TIMER = 0x10, // MBC3 with Battery + RAM + Timer

	// MBC4 does not exist.

	MBC5 = 0x19, // Up to 8MiB ROM
	MBC5_RAM = 0x1A, // MBC5 with RAM
	MBC5_BAT_RAM = 0x1B, // MBC5 with Battery RAM
	MBC5_RUMBLE = 0x1C, // MBC5 with Rumble (rumble not emulated)
	MBC5_RUMBLE_RAM = 0x1D, // MBC5 with Rumble and RAM
	MBC5_RUMBLE_BAT_RAM = 0x1E, // MBC5 with Rumble, Battery, and RAM

	MBC6 = 0x20, // Used in one game that can't be emulated anyway

	// MBC7 is used for an accelerometer, which I don't plan on emulating.
	// I will not be emulating MMM or M161 cartridges -IM

	HuC1 = 0xFF, // Seems easy enough to emulate, if I ever find the documentation.
	HuC3 = 0xFE, // Alongside banking, contains a RTC, speaker, and IR Blaster/Reciever. Fancy.
	// Not emulating Wisdom Tree MBC
	// Not emulating BANDAI TAMA5
	// Probably not emulating GB Camera
};

// Constructor
Cartridge::Cartridge(std::string rom_file_path, MMU *mem)
{
	this->rom_file_path = rom_file_path;
	this->sav_file_path = "";
	rom_bank_amount = 0;
	ram_bank_amount = 0;
	mbc_id = 0;

	// Check to see if the rom_file_path is valid and accessible
	if(!std::filesystem::exists(rom_file_path))
	{
		throw std::invalid_argument("File does not exist.");
	}

	// Check to see that the file is a .gb file
	unsigned long rom_extension_pos = rom_file_path.find_last_of('.');
	std::string rom_extension = rom_file_path.substr(rom_extension_pos);
	if(rom_extension != ".gb")
	{
		throw std::invalid_argument("File is not a .gb Gameboy ROM.");
	}

	// Assuming file is readable, since it passed exist() check
	RomFile.open(rom_file_path, std::ios::binary);

	// Load the ROM file. Throws exceptions which are caught above this.
	loadROM(mem);

	RomFile.close();
}



// Destructor
Cartridge::~Cartridge()
{
	RomFile.close();
}



// Gets the ROM file path
std::string Cartridge::getROMFilePath()
{
	return rom_file_path;
}



// Gets the SAV file path
std::string Cartridge::getSAVFilePath()
{
	return sav_file_path;
}



// Gets the number of ROM Banks
int Cartridge::getROMBankAmount()
{
	return rom_bank_amount;
}


// Gets the number of RAM Banks
int Cartridge::getRAMBankAmount()
{
	return ram_bank_amount;
}



// Loads the RomFile from the rom_file_path
void Cartridge::loadROM(MMU *mem)
{
	// Read the ROM's header into a byte array
	std::array<uint8_t, 80> header{}; // Header is 80 bytes between $100-$14F
	try {
		RomFile.seekg(0x100);
		RomFile.read( (char*)(header.data()), 80);
		RomFile.seekg(0); // Reset position

	} catch(std::exception& ex) {
		Logger::instance().log("CART: Could not read ROM Header.",
							   Logger::ERRORS);
		Logger::instance().log(ex.what(), Logger::ERRORS);

		// Rethrow with more descriptive message
		throw std::ios::failure("ROM is corrupt: Could not read ROM Header.");
	}

	// Check the validity of the ROM's header
	if(!isHeaderValid(header))
	{
		throw std::ios::failure("ROM is corrupt: Bad header checksum.");
	}

	game_title = parseGameTitle(header);

	// Get the Memory Bank Controller from the header
	mbc_id = header.at(0x47);

	// Check to see if external RAM is persistent
	bool persistent = false;

	switch(mbc_id)
	{
	case NONE_BAT_RAM: case MBC1_BAT_RAM: case MBC2_BAT: case MBC3_BAT_RAM:
	case MBC3_BAT_RAM_TIMER: case MBC5_BAT_RAM: case MBC5_RUMBLE_BAT_RAM:
	{
		persistent = true;
		sav_file_path = createSAVFilePath();
	}
	}

	// Get the ROM and RAM bank amounts
	switch(header.at(0x48))
	{
	case 0x00: rom_bank_amount = 2; break;
	case 0x01: rom_bank_amount = 4; break;
	case 0x02: rom_bank_amount = 8; break;
	case 0x03: rom_bank_amount = 16; break;
	case 0x04: rom_bank_amount = 32; break;
	case 0x05: rom_bank_amount = 64; break;
	case 0x06: rom_bank_amount = 128; break;
	case 0x07: rom_bank_amount = 256; break;
	case 0x08: rom_bank_amount = 512; break;
	// 0x52, 0x53, and 0x54 might exist, but probably don't.
	}

	switch(header.at(0x49))
	{
	case 0x00: ram_bank_amount = 0; break;
	// 0x01 is unused
	case 0x02: ram_bank_amount = 1; break;
	case 0x03: ram_bank_amount = 4; break;
	case 0x04: ram_bank_amount = 16; break;
	case 0x05: ram_bank_amount = 8; break;
	}

	// Send Save file info to MMU
	mem->setERAM(ram_bank_amount, persistent, sav_file_path);


	// Begin loading ROM data into MMU
	try {
		std::array<uint8_t, 0x4000> static_rom{};
		RomFile.read((char*)(static_rom.data()), static_rom.size());

		mem->setROM1(static_rom);

	} catch(std::exception& ex) {

		throw std::ios::failure("ROM is corrupt: Could not read static ROM");
	}

	// Load ROM banks into memory
	// Subtract 1 because Static ROM (bank 1) is already loaded
	std::vector< std::array<uint8_t, 0x4000> > rom_banks{};

	// Iterate through every bank, reading 16kb chunks into memory
	for(int i = 0; i < rom_bank_amount - 1; i++)
	{
		try {
			std::array<uint8_t, 0x4000> bank{};
			RomFile.read((char*)(bank.data()), bank.size());
			rom_banks.push_back(bank);

		} catch(std::exception& ex) {
			throw std::ios::failure(
					"ROM is corrupt: Could not read all ROM banks.");
		}
	}

	// Send banks to MMU
	mem->setROM2(rom_banks, rom_bank_amount);
}



// Returns if the ROM header is valid.
bool Cartridge::isHeaderValid(std::array<uint8_t, 80>& header)
{
	// The checked header resides in 25 bytes from $134-$14C
	// The checksum value is at $14D
	std::array<uint8_t, 25> checked_header{};
	std::copy(
			header.begin() + 0x34,
			header.begin() + 0x4C + 1,
			checked_header.begin()
	);
	uint8_t checksum = header.at(0x4D);

	// The checksum is calculated by adding the inverse of all values inside
	// the checked header.
	uint8_t sum = 0;
	for(uint8_t value : checked_header) { sum += ~value; }

	// The header is valid if the sum is equal to the checksum
	return sum == checksum;
}



// Gets the game title from the ROM header
std::string Cartridge::parseGameTitle(std::array<uint8_t, 80>& header)
{
	std::string title;
	// The game title is stored in ASCII at location $133-$143, usually.
	std::copy(header.begin() + 0x34,
			  header.begin() + 0x43,
			  std::back_inserter(title)
			  );

	return title;
}



std::string Cartridge::createSAVFilePath()
{
	// Remove the previous extension
	unsigned long rom_extension_pos = rom_file_path.find_last_of('.');
	std::string sav_path = rom_file_path.substr(0, rom_extension_pos);
	// Add .sav as extension
	sav_path.append(".sav");

	return sav_path;
}

