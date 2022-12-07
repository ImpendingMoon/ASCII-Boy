/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbstructs.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
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


// FLAGREGISTER CLASS //


// Converts a byte (f register) into flag booleans
FlagRegister::FlagRegister()
{
	zero = false;
	subtract = false;
	half_carry = false;
	carry = false;
}


// Converts the flags into a byte (f register)
uint8_t FlagRegister::flagsToByte()
{
	uint8_t output = 0;
	output |= (zero << ZERO_POSITION);
	output |= (subtract << SUBTRACT_POSITION);
	output |= (half_carry << HALF_CARRY_POSITION);
	output |= (carry << CARRY_POSITION);
	return output;
}


// Converts the flags into a byte (f register)
void FlagRegister::byteToFlags(uint8_t f_reg)
{
	zero = (f_reg >> ZERO_POSITION) & 1;
	subtract = (f_reg >> SUBTRACT_POSITION) & 1;
	half_carry = (f_reg >> HALF_CARRY_POSITION) & 1;
	carry = (f_reg >> CARRY_POSITION) & 1;
}

// END FLAGREGISTER CLASS //
