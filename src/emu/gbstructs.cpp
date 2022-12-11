/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/gbstructs.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Structs and Enums used by components of the Gameboy. Includes enums to strings
 ******************************************************************************/

#include "gbstructs.hpp"

using namespace gbstructs;

// Converts enums to strings.
std::string gbstructs::enumToString(TargetID id)
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



// Creates a formatted string from a RegisterSet
std::string gbstructs::registerToString(RegisterSet regs) {
	std::string output{};

	output.append("(");
	output.append(fmt::format("A: 0x{:02X} | ", regs.a));
	output.append(fmt::format("F: 0b{:08b} | ", regs.f));
	output.append(fmt::format("B: 0x{:02X} | ", regs.b));
	output.append(fmt::format("C: 0x{:02X} | ", regs.c));
	output.append(fmt::format("D: 0x{:02X} | ", regs.d));
	output.append(fmt::format("E: 0x{:02X} | ", regs.e));
	output.append(fmt::format("H: 0x{:02X} | ", regs.h));
	output.append(fmt::format("L: 0x{:02X} | ", regs.l));
	output.append(fmt::format("SP: 0x{:04X} | ", regs.sp));
	output.append(fmt::format("PC: 0x{:04X}", regs.pc));
	output.append(")");

	return output;
}



// Converts a 3-bit register ID to a TargetID
std::string gbstructs::instructionToString(CPUInstruction ins)
{
	std::string output{};

	output.append("(");
	output.append("INS: " + ins.mnemonic + " | ");
	output.append("Target1: " + enumToString(ins.target1) + " | ");
	output.append("Target2: " + enumToString(ins.target2) + " | ");
	output.append(fmt::format("T1 Address: {} | ", ins.t1_as_address));
	output.append(fmt::format("T2 Address: {} | ", ins.t2_as_address));
	output.append(fmt::format("Origin Address: ${:04X} | ", ins.origin));
	if(ins.two_byte)
	{
		output.append(fmt::format("Opcode: 0x{:04X}", ins.opcode));
	} else {
		output.append(fmt::format("Opcode: 0x{:02X}", ins.opcode));
	}
	output.append(")");

	return output;
}




// FLAGREGISTER CLASS //


// Converts a byte (f register) into flag booleans
gbstructs::FlagRegister::FlagRegister()
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
