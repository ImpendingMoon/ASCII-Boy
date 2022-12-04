/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbstructs.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 3 Dec 2022
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
