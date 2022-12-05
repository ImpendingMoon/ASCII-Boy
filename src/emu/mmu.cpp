/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/mmu.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 4 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Memory Management Unit of the Gameboy. Handles memory IO for other components.
 ******************************************************************************/

#include "mmu.hpp"

MMU::MMU()
{
	// All banked memory is handled by the cartridge when loading a ROM file
	// This is because those memory chips are handled by the cartridge.
	ROM2 = nullptr;
	ERAM = nullptr;

	// Initialize memory
	// This isn't required, but makes logging and debugging easier

	// ROM1
	for(int i = 0; i < sizeof ROM1; i++) { ROM1[i] = 0; }
	// VRAM
	for(int i = 0; i < sizeof VRAM; i++) { VRAM[i] = 0; }
	// WRAM
	for(int i = 0; i < sizeof WRAM; i++) { WRAM[i] = 0; }
	// OAM
	for(int i = 0; i < sizeof OAM; i++) { OAM[i] = 0; }
	// IOReg
	for(int i = 0; i < sizeof IOReg; i++) { IOReg[i] = 0; }
	// HRAM
	for(int i = 0; i < sizeof HRAM; i++) { HRAM[i] = 0; }

	IEReg = 0x00;
}
