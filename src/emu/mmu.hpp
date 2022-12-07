/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/mmu.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Memory Management Unit of the Gameboy. Handles memory IO for other components.
 ******************************************************************************/

#pragma once

#include "../core.hpp"

class Cartridge;
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
	uint8_t writeByte(uint16_t address, uint8_t value);
	// Writes a byte to memory, ignoring PPU locks
	uint8_t writeByte(uint16_t address, uint8_t value, bool is_ppu);

	// Reads a byte from memory, without logging. For dumping memory.
	uint8_t getByte(uint16_t address);

	// Sets the current ROM2 bank
	void setROM2Index(int index);
	// Gets the current ROM2 bank
	int getROM2Index();
	// Sets the current ERAM bank
	void setERAMIndex(int index);
	// Gets the current ERAM index
	int getERAMIndex();

	// Sets ROM2 to a 2D array of bytes
	void setROM2(uint8_t** banks);

	// Sets the ORAM_locked state
	void setORAMLocked(bool value);
	// Gets the ORAM_locked state
	bool getORAMLocked();
	// Sets the VRAM_locked state
	void setVRAMLocked(bool value);
	// Gets the VRAM_locked state
	bool getVRAMLocked();

	// Dumps the entire memory address space into a formatted string.
	std::string dumpMemory();

private:
	// Memory banks
	uint8_t ROM1[0x4000]; // Static ROM $0000-$3FFF

	uint8_t** ROM2; // Banked ROM $4000-$7FFF
	int ROM2_index; // Which ROM2 bank the MMU uses

	uint8_t VRAM[0x2000]; // VRAM $8000-9FFF

	// External RAM $A000-BFFF. Handled by Cartridge
	int ERAM_index; // Which ERAM bank the MMU uses

	uint8_t WRAM[0x2000]; // Work RAM $C000-DFFF - Banked in CGB

	// Echo RAM $E000-$FDFF
	
	uint8_t OAM[0x100]; // Object Attribute Memory $FE00-$FE9F

	// Unmapped memory $FEA0-FEFF
	
	uint8_t IOReg[0x80]; // I/O Registers $FF00-FF7F
					  
	uint8_t HRAM[0x80]; // HRAM $FF80-$FFFE
					 
	uint8_t IEReg; // Interrupt Enable Register $FFFF

	// ORAM and VRAM access is locked during some PPU states
	bool ORAM_locked;
	bool VRAM_locked;
};
