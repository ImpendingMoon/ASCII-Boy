/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/mmu.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 9 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Memory Management Unit of the Gameboy. Handles memory IO for other components.
 ******************************************************************************/

#pragma once

#include "../core.hpp"
#include "gbstructs.hpp"

class MMU
{
public:
	MMU();
	~MMU();

	// Reads a byte from memory
	uint8_t readByte(uint16_t address);
	// Reads a byte from memory, ignoring PPU locks
	uint8_t readByte(uint16_t address, bool is_ppu);
	// Writes a byte to memory, if legal
	void writeByte(uint16_t address, uint8_t value);
	// Writes a byte to memory, ignoring PPU locks
	void writeByte(uint16_t address, uint8_t value, bool is_ppu);

	// Sets the current ROM2 bank
	void setROM2Index(int index);
	// Gets the current ROM2 bank
	int getROM2Index();
	// Sets the current ERAM bank
	void setERAMIndex(int index);
	// Gets the current ERAM index
	int getERAMIndex();

	// Sets ROM1 to an array of bytes
	void setROM1(std::array<uint8_t, 0x4000>& bank);
	// Sets ROM2 to a 2D array of bytes
	void setROM2(std::vector<std::array<uint8_t, 0x4000>>& banks,
				 int bank_amount);
	// Sets and initializes ERAM
	void setERAM(int bank_amount,
				 bool persistent,
				 std::string sav_file_path,
				 int mbc_id);

	// Sets the ORAM_locked state
	void setOAMLocked(bool value);
	// Gets the ORAM_locked state
	bool getOAMLocked();
	// Sets the VRAM_locked state
	void setVRAMLocked(bool value);
	// Gets the VRAM_locked state
	bool getVRAMLocked();

	// Dumps the entire memory address space into a formatted string.
	std::string dumpMemory();

private:
	// Memory banks
	std::array<uint8_t, 0x4000> ROM1{}; // Static ROM $0000-$3FFF

	std::vector< std::array<uint8_t, 0x4000> > ROM2{}; // Banked ROM $4000-$7FFF
	int ROM2_index; // Which ROM2 bank the MMU uses
	int ROM2_bank_amount{}; // Amount of ROM banks that exist

	std::array<uint8_t, 0x4000> VRAM{}; // VRAM $8000-$9FFF

	// External RAM $A000-BFFF.
	// Either a file on disk if persistent, or just an array if not.
	std::string sav_file_path;
	// TODO: This should be a memory mapped file instead.
	int mbc;
	std::fstream SavFile;
	bool ERAM_persistent;
	std::vector< std::array<uint8_t, 0x2000> > ERAM{};
	int ERAM_index; // Which ERAM bank the MMU uses
	int ERAM_bank_amount{}; // Amount of ERAM banks that exist

	std::array<uint8_t, 0x2000> WRAM{}; // Work RAM $C000-$DFFF

	// Echo RAM $E000-$FDFF
	
	std::array<uint8_t, 0x100> OAM{}; // Object Attribute Memory $FE00-$FE9F

	// Unmapped memory $FEA0-FEFF
	
	std::array<uint8_t, 0x80> IOReg{}; // I/O Registers $FF00-$FF7F
					  
	std::array<uint8_t, 0x80> HRAM{}; // HRAM $FF80-$FFFE
					 
	uint8_t IEReg; // Interrupt Enable Register $FFFF

	// ORAM and VRAM access is locked during some PPU states
	bool OAM_locked;
	bool VRAM_locked;

	// Reads a byte from memory, without logging. For dumping memory.
	inline uint8_t getByte(uint16_t address);

	// Reads a byte from external RAM
	uint8_t readERAMByte(int bank, uint16_t address);
	// Writes a byte to external RAM
	void writeERAMByte(int bank, uint16_t address, uint8_t value);
};
