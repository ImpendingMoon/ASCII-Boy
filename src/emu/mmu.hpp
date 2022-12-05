/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/mmu.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 4 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Memory Management Unit of the Gameboy. Handles memory IO for other components.
 ******************************************************************************/

#pragma once

#include "../core.hpp"

class MMU
{
public:
	MMU();
private:
	// Memory banks
	byte ROM1[0x4000];  // Static ROM $0000-$3FFF
	byte** ROM2; // Banked ROM $4000-$7FFF
	byte VRAM[0x2000]; // VRAM $8000-9FFF
	byte** ERAM; // External RAM $A000-BFFF
	byte WRAM[0x2000]; // Work RAM $C000-DFFF
	// Echo RAM $E000-$FDFF
	byte OAM[0x100]; // Object Attribute Memory $FE00-$FE9F
	// Unmapped memory $FEA0-FEFF
	byte IOReg[0x80]; // I/O Registers $FF00-FF7F
	byte HRAM[0x80]; // HRAM $FF80-$FFFE
	byte IEReg; // Interrupt Enable Register $FFFF

};
