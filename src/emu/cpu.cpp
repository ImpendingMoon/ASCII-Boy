/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cpu.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 The main CPU of the Gameboy. Handles registers, opcode execution, and cycles.
 ******************************************************************************/

#include "cpu.hpp"

CPU::CPU()
{
	// Set initial register values
	// Equivalent to DMG0 values
	regs.a = 0x01;
	regs.f = 0x00;
	regs.b = 0xFF;
	regs.c = 0x13;
	regs.d = 0x00;
	regs.e = 0xC1;
	regs.h = 0x84;
	regs.l = 0x93;
	regs.sp = 0xFFFE;
	regs.pc = 0x0100;

	halted = false;
	next_halted_state = false;
}


// Executes an opcode, returns the number of cycles used
int CPU::execute(uint8_t opcode, MMU &mem)
{
	using namespace emath;

	// Setup

	int cycles = 0;
	bool done = false;
	CPUInstruction ins{};

	flags.byteToFlags(regs.f);
	regs.pc++;



	// Decode/Execute

	// Algorithmic Checks //

	// LD r1,r2 - Load Register 2 into Register 1
	// Opcode 0x76 lies in this range and is a HALT instruction
	if (opcode >= 0b01000000 && opcode <= 0b01111111 && opcode != 0x76)
	{
		ins.mnemonic = "LD";
		ins.target1 = toTarget((opcode & 0b00111000) >> 3);
		ins.target2 = toTarget(opcode & 0b00000111);

		if (ins.target1 != HL && ins.target2 != HL)
		{
			// Move the value of target2 into target1
			uint8_t value = getByteReg(ins.target2);
			setByteReg(ins.target1, value);

		} else if (ins.target1 == HL) { // HL is an address

			// Write to address $HL, value in t2
			ins.t1_as_address = true;

			uint8_t value = getByteReg(ins.target2);
			mem.writeByte(regs.hl, value);
			cycles += 4;

		} else if(ins.target2 == HL) { // HL is an address

			// Read to t1, value at address $HL
			ins.t2_as_address = true;

			uint8_t value = mem.readByte(regs.hl);
			setByteReg(ins.target1, value);
			cycles += 4;

		}

		done = true;

	} // END LD r1,r2

	// ADD A,r - Add register 'r' into A
	if(!done && opcode >= 0b10000000 && opcode <= 0b10000111)
	{
		ins.mnemonic = "ADD";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t sum = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2);

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl);
			cycles += 4;

		}

		sum += value;
		regs.a = sum;

		flags.zero = (regs.a == 0);
		flags.subtract = false;
		flags.half_carry = checkHCAdd(regs.a, value);
		flags.carry = checkOFAdd(regs.a, value);

		done = true;

	} // END ADD A,r

	// ADC A,r - Add register 'r' + carry flag into A
	if(!done && opcode >= 0b10001000 && opcode <= 0b10001111)
	{
		ins.mnemonic = "ADC";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t sum = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2) + flags.carry;

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl) + flags.carry;
			cycles += 4;

		}

		sum += value;
		regs.a = sum;

		flags.zero = (regs.a == 0);
		flags.subtract = false;
		flags.half_carry = checkHCAdd(regs.a, value);
		flags.carry = checkOFAdd(regs.a, value);

		done = true;
	} // END ADC A,r

	// SUB A,r - Subtract register 'r' from A
	if(!done && opcode >= 0b10010000 && opcode <= 0b10010111)
	{
		ins.mnemonic = "SUB";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t dif = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2);

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl);
			cycles += 4;

		}

		dif -= value;
		regs.a = dif;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = checkHCSub(regs.a, value);
		flags.carry = checkUFSub(regs.a, value);

		done = true;

	} // END SUB A,r

	// SBC A,r - Subtract (register 'r' + carry flag) from A
	if(!done && opcode >= 0b10011000 && opcode <= 0b10011111)
	{
		ins.mnemonic = "SBC";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t dif = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2) + flags.carry;

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl) + flags.carry;
			cycles += 4;

		}

		dif -= value;
		regs.a = dif;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = checkHCSub(regs.a, value);
		flags.carry = checkUFSub(regs.a, value);

		done = true;

	} // END SBC A,r

	// AND A,r - Mask A with value of register 'r' using AND
	if(!done && opcode >= 0b10100000 && opcode <= 0b10100111)
	{
		ins.mnemonic = "AND";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t val = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2);

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl);
			cycles += 4;

		}

		val &= value;
		regs.a = val;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = true;
		flags.carry = false;

		done = true;

	} // END AND A,r

	// OR A,r - Mask A with value of register 'r' using OR
	if(!done && opcode >= 0b10110000 && opcode <= 0b10110111)
	{
		ins.mnemonic = "OR";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t val = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2);

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl);
			cycles += 4;

		}

		val |= value;
		regs.a = val;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = true;
		flags.carry = false;

		done = true;

	} // END OR A,r

	// XOR A,r - Mask A with value of register 'r' using XOR
	if(!done && opcode >= 0b10101000 && opcode <= 0b10101111)
	{
		ins.mnemonic = "XOR";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t val = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2);

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl);
			cycles += 4;

		}

		val ^= value;
		regs.a = val;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = true;
		flags.carry = false;

		done = true;

	} // END XOR A,r

	// CP A,r - Compares A with value in register 'r'. (SUB without changing A)
	if(!done && opcode >= 0b10111000 && opcode <= 0b10111111)
	{
		ins.mnemonic = "SUB";
		ins.target1 = A;
		ins.target2 = toTarget(opcode & 0b00000111);

		uint8_t value = 0;
		uint8_t dif = regs.a;

		if(ins.target2 != HL)
		{
			value = getByteReg(ins.target2);

		} else if(ins.target2 == HL) { // HL is an address

			ins.t2_as_address = true;
			value = mem.readByte(regs.hl);
			cycles += 4;

		}

		dif -= value;

		flags.zero = (dif == 0);
		flags.subtract = true;
		flags.half_carry = checkHCSub(dif, value);
		flags.carry = checkUFSub(dif, value);

		done = true;

	} // END CP A,r

	// End Algorithmic Checks //



	// Two-Byte instructions //

	if(!done && opcode == 0xCB)
	{
		ins.two_byte = true;

		// New opcode is immediate value
		opcode = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		// Change opcode for logging
		ins.opcode = (ins.opcode << 8) | opcode;



		// SWAP r - Swap the upper and lower nibbles of a byte
		if(opcode >= 0b00110000 && opcode <= 0b00110111)
		{
			ins.mnemonic = "SWAP";
			ins.target1 = toTarget(opcode & 0b00000111);

			uint8_t value;

			if(ins.target1 != HL)
			{
				value = getByteReg(ins.target1);

				uint8_t unibble = value & 0b11110000;
				uint8_t lnibble = value & 0b00001111;

				value = (unibble >> 8) | (lnibble << 8);

				setByteReg(ins.target1, value);

			} else { // HL is an address

				ins.t1_as_address = true;
				value = mem.readByte(regs.hl);
				cycles += 4;

				uint8_t unibble = value & 0b11110000;
				uint8_t lnibble = value & 0b00001111;

				value = (unibble >> 8) | (lnibble << 8);

				mem.writeByte(regs.hl, value);
				cycles += 4;

			}

			// Zero is unchanged
			flags.subtract = false;
			flags.half_carry = false;
			flags.carry = false;

			done = true;

		} // END SWAP
	}

	// End Two-Byte instructions //



	// TODO: The rest of the instructions



	// Final logging/cleanup

	if(!done)
	{
		Logger::instance().log(
				fmt::format("CPU: Unhandled instruction 0x{:02X}!",
									opcode),
				Logger::DEBUG
				);
	} else {

		Logger::instance().log(
				fmt::format("CPU: Executed instruction {0}",
									instructionToString(ins)),
				Logger::EXTREME
				);
		Logger::instance().log(
				fmt::format("CPU: New register state {0}",
									registerToString(regs)),
				Logger::EXTREME
				);
	}

	regs.f = flags.flagsToByte(); // Set flags register
	cycles += 4; // Every instruction takes at least 4 cycles

	return cycles;
}


// Gets a byte from an 8-bit register
uint8_t CPU::getByteReg(TargetID target)
{
	switch(target)
	{
	case A: return regs.a;
	case F: return regs.f;
	case B: return regs.b;
	case C: return regs.c;
	case D: return regs.d;
	case E: return regs.e;
	case H: return regs.h;
	case L: return regs.l;
	default:
	{
		throw new std::invalid_argument(
				  "Attempted read of invalid target in getByteReg()"
				  );
	}
	}
}

// Sets an 8-bit register to a value
void CPU::setByteReg(TargetID target, uint8_t value)
{
	switch(target)
	{
		case A: regs.a = value; return;
		case F: regs.f = value; return;
		case B: regs.b = value; return;
		case C: regs.c = value; return;
		case D: regs.d = value; return;
		case E: regs.e = value; return;
		case H: regs.h = value; return;
		case L: regs.l = value; return;
		default:
		{
			throw new std::invalid_argument(
					"Attempted write of invalid target in setByteReg()"
					);
		}
	}
}

// Gets a short from a 16-bit register
uint16_t CPU::getShortReg(TargetID target)
{
	switch(target)
	{
	case AF: return regs.af;
	case BC: return regs.bc;
	case DE: return regs.de;
	case HL: return regs.hl;
	case SP: return regs.sp;
	case PC: return regs.pc;
	default:
	{
		throw new std::invalid_argument(
				"Attempted read of invalid target in getShortReg()"
				);
	}
	}
}

// Sets a 16-bit register to a value
void CPU::setShortReg(TargetID target, uint16_t value)
{
	switch(target)
	{
	case AF: regs.af = value; return;
	case BC: regs.bc = value; return;
	case DE: regs.de = value; return;
	case HL: regs.hl = value; return;
	case SP: regs.sp = value; return;
	case PC: regs.pc = value; return;
	default:
	{
		throw new std::invalid_argument(
				"Attempted write of invalid target in setShortReg()"
				);
	}
	}
}



// Converts a 3-bit ID to a TargetID
TargetID CPU::toTarget(uint8_t id)
{
	// 111 = RegA // 000 = RegB // 001 = RegC // 010 = RegD
	// 011 = RegE // 100 = RegH // 101 = RegL // 010 = (HL)

	switch (id & 0b00000111)
	{
		case 0b111: return A;
		case 0b000: return B;
		case 0b001: return C;
		case 0b010: return D;
		case 0b011: return E;
		case 0b100: return H;
		case 0b101: return L;
		case 0b110: return HL;
		default:
		{
			// This should be impossible.
			throw new std::logic_error(
					"Invalid ID in toTarget(). If this happens, let Moon know."
					);
		}
	}
}