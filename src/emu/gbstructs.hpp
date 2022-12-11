/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbstructs.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Structs and Enums used by components of the Gameboy. Includes enums to strings
 ******************************************************************************/

#pragma once

#include "../core.hpp"

namespace gbstructs
{
	struct RegisterSet
	{
		// This shouldn't be done, but what can you do?
		// (many things. im just lazy.) -IM

		union { // AF
			uint16_t af;
			struct { uint8_t f, a; };
		};

		union { // BC
			uint16_t bc;
			struct { uint8_t c, b; };
		};

		union { // DE
			uint16_t de;
			struct { uint8_t e, d; };
		};

		union { // HL
			uint16_t hl;
			struct { uint8_t l, h; };
		};

		uint16_t sp;
		uint16_t pc;
	};



	// The flag register is a set of 4 bools
	class FlagRegister
	{
	public:
		bool zero;
		bool subtract;
		bool half_carry;
		bool carry;

		static constexpr int ZERO_POSITION = 7;
		static constexpr int SUBTRACT_POSITION = 6;
		static constexpr int HALF_CARRY_POSITION = 5;
		static constexpr int CARRY_POSITION = 4;

		FlagRegister();

		// Converts a byte (f register) into flag booleans
		void byteToFlags(uint8_t f_reg);
		// Converts the flags into a byte (f register)
		uint8_t flagsToByte();
	};


	// Used to generalize targets for instructions. Also useful for logging.
	enum TargetID
	{
		NOTARGET,
		A, F, B, C, D, E, H, L,
		AF, BC, DE, HL,
		SP, PC,
	};

	// Used to indicate MBC controls and memory persistence
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

	// Converts enums to strings.
	std::string enumToString(TargetID id);
}
