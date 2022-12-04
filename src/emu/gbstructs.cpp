/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbstructs.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 3 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Structs and Enums used by components of the Gameboy. Includes enums to strings
 ******************************************************************************/

#include "gbstructs.hpp"

using namespace gbstructs;

// Converts enums to strings.
std::string enumToString(TargetID id)
{
	std::string output = "ERROR";
	switch(id)
	{
	case NOTARGET: output = "NOTARGET"; break;
	case A: output = "A"; break;
	case F: output = "F"; break;
	case B: output = "B"; break;
	case C: output = "C"; break;
	case D: output = "D"; break;
	case E: output = "E"; break;
	case H: output = "H"; break;
	case L: output = "L"; break;
	case AF: output = "AF"; break;
	case BC: output = "BC"; break;
	case DE: output = "DE"; break;
	case HL: output = "HL"; break;
	case SP: output = "SP"; break;
	case PC: output = "PC"; break;
	}
	return output;
}

