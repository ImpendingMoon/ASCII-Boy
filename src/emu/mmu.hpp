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
	byte readByte(ushort address);
	// Reads a byte from memory, ignoring PPU locks
	byte readByte(ushort address, bool is_ppu);
	// Writes a byte to memory, if legal
	byte writeByte(ushort address, byte value);
	// Writes a byte to memory, ignoring PPU locks
	byte writeByte(ushort address, byte value, bool is_ppu);

	// Reads a byte from memory, without logging. For dumping memory.
	byte getByte(ushort address);

	// Sets the current ROM2 bank
	void setROM2Index(int index);
	// Gets the current ROM2 bank
	int getROM2Index();
	// Sets the current ERAM bank
	void setERAMIndex(int index);
	// Gets the current ERAM index
	int getERAMIndex();

	// Sets ROM2 to a 2D array of bytes
	void setROM2(byte** banks);

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
	byte ROM1[0x4000]; // Static ROM $0000-$3FFF

	byte** ROM2; // Banked ROM $4000-$7FFF
	int ROM2_index; // Which ROM2 bank the MMU uses

	byte VRAM[0x2000]; // VRAM $8000-9FFF

	// External RAM $A000-BFFF. Handled by Cartridge
	int ERAM_index; // Which ERAM bank the MMU uses

	byte WRAM[0x2000]; // Work RAM $C000-DFFF - Banked in CGB

	// Echo RAM $E000-$FDFF
	
	byte OAM[0x100]; // Object Attribute Memory $FE00-$FE9F

	// Unmapped memory $FEA0-FEFF
	
	byte IOReg[0x80]; // I/O Registers $FF00-FF7F
					  
	byte HRAM[0x80]; // HRAM $FF80-$FFFE
					 
	byte IEReg; // Interrupt Enable Register $FFFF

	// ORAM and VRAM access is locked during some PPU states
	bool ORAM_locked;
	bool VRAM_locked;
};
