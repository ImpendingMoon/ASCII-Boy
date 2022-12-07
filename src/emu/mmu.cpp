/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/mmu.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Memory Management Unit of the Gameboy. Handles memory IO for other components.
 ******************************************************************************/

#include "mmu.hpp"


// Constructor
MMU::MMU()
{
	// All banked memory is handled by the cartridge when loading a ROM file
	ROM2 = nullptr;
	ROM2_index = 0;
	ERAM_index = 0;

	// Initialize memory
	// This isn't required, but makes logging and debugging easier

	// ROM1
	for(ulong i = 0; i < sizeof ROM1; i++) { ROM1[i] = 0; }
	// VRAM
	for(ulong i = 0; i < sizeof VRAM; i++) { VRAM[i] = 0; }
	// WRAM
	for(ulong i = 0; i < sizeof WRAM; i++) { WRAM[i] = 0; }
	// OAM
	for(ulong i = 0; i < sizeof OAM; i++) { OAM[i] = 0; }
	// IOReg
	for(ulong i = 0; i < sizeof IOReg; i++) { IOReg[i] = 0; }
	// HRAM
	for(ulong i = 0; i < sizeof HRAM; i++) { HRAM[i] = 0; }

	IEReg = 0x00;

	ORAM_locked = false;
	VRAM_locked = false;
}

// Destructor
MMU::~MMU()
{
	// Maybe unneeded. If this causes a Segfault while closing, delete this.
	delete ROM2;
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
void MMU::setORAMLocked(bool value)
{
	ORAM_locked = value;
}

// Gets the ORAM_locked state
bool MMU::getORAMLocked()
{
	return ORAM_locked;
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

// Sets ROM2 to a 2D array of bytes
void MMU::setROM2(uint8_t** banks)
{
	ROM2 = banks;
}

// End SGetters //



// Memory Functions //

// Reads a byte from memory
uint8_t MMU::readByte(uint16_t address)
{
	// Check for ECHO RAM
	
}
