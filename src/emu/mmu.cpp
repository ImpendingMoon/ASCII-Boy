/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/mmu.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 18 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Memory Management Unit of the Gameboy. Handles memory IO for other components.
 ******************************************************************************/

#include "mmu.hpp"

// Constructor
MMU::MMU()
{
	ROM2_index = 0;
	ROM2_bank_amount = 0;
	ERAM_index = 0;
	ERAM_bank_amount = 0;
	ERAM_persistent = false;
	mbc = 0;
	IEReg = 0;

	OAM_locked = false;
	VRAM_locked = false;
}

// Destructor
MMU::~MMU()
{
	SavFile.close();
}


// SGetters //

// Sets the current ROM2 Bank
void MMU::setROM2Index(int index)
{
	ROM2_index = index;
}

// Gets the current ROM2 bank
int MMU::getROM2Index()
{
	return ROM2_index;
}

// Sets the current ERAM Bank
void MMU::setERAMIndex(int index)
{
	ERAM_index = index;
}

// Gets the current ERAM bank
int MMU::getERAMIndex()
{
	return ERAM_index;
}

// Sets the ORAM_locked state
void MMU::setOAMLocked(bool value)
{
	OAM_locked = value;
}

// Gets the ORAM_locked state
bool MMU::getOAMLocked()
{
	return OAM_locked;
}

// Sets the VRAM_locked state
void MMU::setVRAMLocked(bool value)
{
	VRAM_locked = value;
}

// Gets the VRAM_locked state
bool MMU::getVRAMLocked()
{
	return VRAM_locked;
}

// Sets ROM1 to an array of bytes
void MMU::setROM1(std::array<uint8_t, 0x4000>& bank)
{
	// Copy the passed bank into ROM1 so that the Cartridge can free its memory
	std::copy(bank.begin(), bank.end(), ROM1.begin());
}

// Sets ROM2 to a 2D array of bytes
void MMU::setROM2(std::vector<std::array<uint8_t, 0x4000>>& banks,
				  int bank_amount)
{
	ROM2 = banks;
	ROM2_bank_amount = bank_amount;
}

// Sets and initializes ERAM
void MMU::setERAM(int bank_amount,
				  bool persistent,
				  const std::string& sav_path,
				  int mbc_id)
{
	ERAM_bank_amount = bank_amount;
	ERAM_persistent = persistent;
	sav_file_path = sav_path;
	mbc = mbc_id;

	if(ERAM_persistent)
	{
		// If persistent, open a .sav file for writing

		// HACK: Figure out a better way to create the file if it doesn't exist.
		std::ofstream temp(sav_file_path, std::fstream::out);
		temp.close();

		// Since this also uses the "in" flag, it will not create the file
		SavFile.open(sav_file_path,
					 std::fstream ::out
					 | std::fstream::in
					 | std::fstream::binary);
		// Check to see that the file was created
		if(!SavFile)
		{
			Logger::log("MEM: Could not open .sav file! "
								   "Saves will not be permanent!",
								   Logger::ERRORS);
		}

		// Ensure that the .sav file is the correct size
		uint64_t target_size = ERAM_bank_amount * 0x2000;
		if(SavFile && std::filesystem::file_size(sav_file_path) < target_size)
		{
				std::filesystem::resize_file(sav_file_path, target_size);
		}
	}


	// Use the volatile memory if the memory isn't persistent or the .sav file
	// could not be opened/created.
	if(!ERAM_persistent || !SavFile)
	{
		for(int i = 0; i < ERAM_bank_amount; i++)
		{
			// Every bank is an array of 0x2000 bytes
			std::array<uint8_t, 0x2000> bank{};
			bank.fill(0);
			ERAM.push_back(bank);
		}
	}

}

// End SGetters //



// Memory Functions //

// Reads a byte from memory
uint8_t MMU::readByte(uint16_t address)
{
	// Call readByte() with is_ppu false
	return readByte(address, false);
}


// Reads a byte from memory, can ignore PPU locks
uint8_t MMU::readByte(uint16_t address, bool is_ppu)
{
	Logger::log(
			fmt::format("MEM: Reading value from ${:04X}.",
						address),
			Logger::EXTREME);

	// Check for ECHO RAM
	if(address >= 0xE000 && address <= 0xFDFF)
	{
		// Echos memory between $C000-$DDFF
		uint16_t relative_address = address - 0x2000;
		return readByte(relative_address, is_ppu);
	}

	// Check for unmapped memory
	if(address >= 0xFEA0 && address <= 0xFEFF)
	{
		Logger::log(
				"MEM: Attempted read of undefined memory.",
				Logger::DEBUG);
		return 0xFF; // Usually returns 0xFF from the bus on hardware
	}

	// ROM1
	if(address <= 0x3FFF)
	{
		return ROM1[address]; // Guaranteed to be in-bounds at compile time
	}

	// ROM2
	if(address >= 0x4000 && address <= 0x7FFF)
	{
		// Bounds checking
		if(ROM2_index < 0 || ROM2_index >= ROM2_bank_amount)
		{
			Logger::log(
					"MEM: Attempted read of invalid ROM2 bank.",
					Logger::DEBUG);

			return 0xFF;
		}

		uint16_t relative_address = address - 0x4000;
		
		// Read byte at address in bank
		return ROM2[ROM2_index][relative_address];
	}

	// VRAM
	if(address >= 0x8000 && address <= 0x9FFF)
	{
		// If VRAM is locked and not accessing as PPU, return 0xFF from bus
		if(VRAM_locked && !is_ppu)
		{
			return 0xFF;
		}

		uint16_t relative_address = address - 0x8000;

		return VRAM[relative_address];
	}

	// External RAM is handled by Cartridge
	if(address >= 0xA000 && address <= 0xBFFF)
	{
		// Bounds checking
		if(ERAM_index < 0 || ERAM_index >= ERAM_bank_amount)
		{
			Logger::log(
					"MEM: Attempted read of invalid ERAM bank.",
					Logger::DEBUG);

			return 0xFF;
		}

		uint16_t relative_address = address - 0xA000;
		return readERAMByte(ERAM_index, relative_address);
	}

	// WRAM
	if(address >= 0xC000 && address <= 0xDFFF)
	{
		uint16_t relative_address = address - 0xC000;

		return WRAM[relative_address];
	}

	// OAM
	if(address >= 0xFE00 && address <= 0xFE9F)
	{
		// If OAM is locked and not accessing as PPU, return 0xFF from bus
		if(OAM_locked && !is_ppu)
		{
			return 0xFF;
		}

		uint16_t relative_address = address - 0xFE00;

		return OAM[relative_address];
	}

	// IOReg
	if(address >= 0xFF00 && address <= 0xFF7F)
	{
		uint16_t relative_address = address - 0xFF00;

		return IOReg[relative_address];
	}

	// HRAM
	if(address >= 0xFF80 && address <= 0xFFFE)
	{
		uint16_t relative_address = address - 0xFF80;

		return HRAM[relative_address];
	}

	// IEReg
	if(address == 0xFFFF)
	{
		return IEReg;
	}

	// This should not be an accessible branch.
	Logger::log(
			"MEM: Invalid address provided to readByte()!",
			Logger::ERRORS);

	return 0xFF;
}



// Writes a byte to memory, if legal
void MMU::writeByte(uint16_t address, uint8_t value)
{
	writeByte(address, value, false);
}


// Writes a byte to memory, can ignore PPU locks
void MMU::writeByte(uint16_t address, uint8_t value, bool is_ppu)
{
	// TODO: Handle writes to ROM as MBC controls

	Logger::log(
			fmt::format("MEM: Writing value 0x{:02X} to ${:04X}.",
						value, address),
			Logger::EXTREME);

	// Check for ECHO RAM
	if(address >= 0xE000 && address <= 0xFDFF)
	{
		// Echos memory between $C000-$DDFF
		uint16_t relative_address = address - 0x2000;
		writeByte(relative_address, value, is_ppu);
		return;
	}

	// Check for unmapped memory
	if(address >= 0xFEA0 && address <= 0xFEFF)
	{
		Logger::log(
				"MEM: Attempted write of undefined memory.",
				Logger::DEBUG);
		return;
	}

	// ROM1
	if(address <= 0x3FFF)
	{
		Logger::log(
				"MEM: Attempted write of ROM1.",
				Logger::DEBUG);
		return;
	}

	// ROM2
	if(address >= 0x4000 && address <= 0x7FFF)
	{
		Logger::log(
				"MEM: Attempted write of ROM2.",
				Logger::DEBUG);

		return;
	}

	// VRAM
	if(address >= 0x8000 && address <= 0x9FFF)
	{
		// If VRAM is locked and not accessing as PPU, return 0xFF from bus
		if(VRAM_locked && !is_ppu)
		{
			return; 
		}

		uint16_t relative_address = address - 0x8000;

		VRAM[relative_address] = value;
		return;
	}

	// External RAM is handled by Cartridge
	if(address >= 0xA000 && address <= 0xBFFF)
	{
		// Bounds checking
		if(ERAM_index < 0 || ERAM_index >= ERAM_bank_amount)
		{
			Logger::log(
					"MEM: Attempted write to invalid ERAM bank.",
					Logger::DEBUG);

			return;
		}

		uint16_t relative_address = address - 0xA000;
		writeERAMByte(ERAM_index, relative_address, value);

		return;
	}

	// WRAM
	if(address >= 0xC000 && address <= 0xDFFF)
	{
		uint16_t relative_address = address - 0xC000;

		WRAM[relative_address] = value;
		return;
	}

	// OAM
	if(address >= 0xFE00 && address <= 0xFE9F)
	{
		// If OAM is locked and not accessing as PPU, return 0xFF from bus
		if(OAM_locked && !is_ppu)
		{
			return;
		}

		uint16_t relative_address = address - 0xFE00;

		OAM[relative_address] = value;
		return;
	}

	// IOReg
	if(address >= 0xFF00 && address <= 0xFF7F)
	{
		uint16_t relative_address = address - 0xFF00;

		IOReg[relative_address] = value;
		return;
	}

	// HRAM
	if(address >= 0xFF80 && address <= 0xFFFE)
	{
		uint16_t relative_address = address - 0xFF80;

		HRAM[relative_address] = value;
		return;
	}

	// IEReg
	if(address == 0xFFFF)
	{
		IEReg = value;
		return;
	}

	// This should not be an accessible branch.
	Logger::log(
			"MEM: Invalid address provided to writeByte()!",
			Logger::ERRORS);
}



// Reads a byte from memory, without logging. For dumping memory.
inline uint8_t MMU::getByte(uint16_t address)
{
	// Check for ECHO RAM
	if(address >= 0xE000 && address <= 0xFDFF)
	{
		// Echos memory between $C000-$DDFF
		uint16_t relative_address = address - 0x2000;
		return getByte(relative_address);
	}

	// Check for unmapped memory
	if(address >= 0xFEA0 && address <= 0xFEFF)
	{
		return 0x00;
	}

	// ROM1
	if(address <= 0x3FFF)
	{
		return ROM1[address]; // Guaranteed to be in-bounds at compile time
	}

	// ROM2
	if(address >= 0x4000 && address <= 0x7FFF)
	{
		// Bounds checking
		if(ROM2_index < 0 || ROM2_index >= ROM2_bank_amount)
		{
			return 0x00;
		}

		uint16_t relative_address = address - 0x4000;
		
		// Read byte at address in bank
		return ROM2[ROM2_index][relative_address];
	}

	// VRAM
	if(address >= 0x8000 && address <= 0x9FFF)
	{
		uint16_t relative_address = address - 0x8000;

		return VRAM[relative_address];
	}

	// External RAM is handled by Cartridge
	if(address >= 0xA000 && address <= 0xBFFF)
	{
		// Bounds checking
		if(ERAM_index < 0 || ERAM_index >= ERAM_bank_amount)
		{
			return 0x00;
		}

		uint16_t relative_address = address - 0xA000;
		return readERAMByte(ERAM_index, relative_address);
	}

	// WRAM
	if(address >= 0xC000 && address <= 0xDFFF)
	{
		uint16_t relative_address = address - 0xC000;

		return WRAM[relative_address];
	}

	// OAM
	if(address >= 0xFE00 && address <= 0xFE9F)
	{
		uint16_t relative_address = address - 0xFE00;

		return OAM[relative_address];
	}

	// IOReg
	if(address >= 0xFF00 && address <= 0xFF7F)
	{
		uint16_t relative_address = address - 0xFF00;

		return IOReg[relative_address];
	}

	// HRAM
	if(address >= 0xFF80 && address <= 0xFFFE)
	{
		uint16_t relative_address = address - 0xFF80;

		return HRAM[relative_address];
	}

	// IEReg
	if(address == 0xFFFF)
	{
		return IEReg;
	}

	// This should not be an accessible branch.
	Logger::log(
			"MEM: Invalid address provided to getByte()!",
			Logger::ERRORS);

	return 0x00;
}



// Reads a byte from external RAM
uint8_t MMU::readERAMByte(int bank, uint16_t address)
{
	if(bank > ERAM_bank_amount)
	{
		Logger::log("MEM: Attempted read of invalid ERAM bank.",
							   Logger::DEBUG);
		return 0xFF;
	}

	// If persistent, read from the SAV file
	if(ERAM_persistent && SavFile)
	{
		int absolute_address = bank * address;

		SavFile.seekg(absolute_address);
		return SavFile.get();
	}

	// Otherwise, read from the vector
	return ERAM[bank][address];
}



// Writes a byte to external RAM
void MMU::writeERAMByte(int bank, uint16_t address, uint8_t value)
{
	if(bank > ERAM_bank_amount)
	{
		Logger::log("MEM: Attempted write of invalid ERAM bank.",
							   Logger::DEBUG);
		return;
	}

	// If persistent, write to the SAV file
	if(ERAM_persistent && SavFile)
	{
		int absolute_address = bank * address;

		SavFile.seekp(absolute_address);
		SavFile.put((char)(value));
		return;
	}

	// Otherwise, write to the vector
	ERAM[bank][address] = value;
}



// Dumps the entire memory address space into a formatted string.
std::string MMU::dumpMemory()
{
	std::string output{};

	output.append("--BEGIN MEMORY DUMP--\n");

	// Iterate through entire address space
	for(int i = 0; i <= 0xFFFF; i++)
	{
		// Add newline and address every 32 bytes
		if(i % 32 == 0)
		{
			output.append( fmt::format("\n${:04X} ", i) );
		}

		uint8_t value = getByte(i);
		// Format data as hexadecimal number
		output.append( fmt::format("{:02X} ", value) );
	}

	output.append("\n\n--END MEMORY DUMP--");

	return output;
}
