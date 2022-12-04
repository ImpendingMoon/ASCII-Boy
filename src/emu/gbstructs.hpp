/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbstructs.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 4 Dec 2022
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
			ushort af;
			struct { byte f, a; };
		};

		union { // BC
			ushort bc;
			struct { byte c, b; };
		};

		union { // DE
			ushort de;
			struct { byte e, d; };
		};

		union { // HL
			ushort hl;
			struct { byte l, h; };
		};

		ushort sp;
		ushort pc;
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
		void byteToFlags(byte f_reg);
		// Converts the flags into a byte (f register)
		byte flagsToByte();
	};


	// Used to generalize targets for instructions. Also useful for logging.
	enum TargetID
	{
		NOTARGET,
		A, F, B, C, D, E, H, L,
		AF, BC, DE, HL,
		SP, PC,
	};

	// Converts enums to strings.
	std::string enumToString(TargetID id);
}
