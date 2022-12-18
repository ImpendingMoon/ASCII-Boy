/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : emu/cpu.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 18 Dec 2022
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
	interrupts_enabled = true;
	next_interrupt_state = true;
}


// Executes an opcode, returns the number of cycles used
int CPU::execute(uint8_t opcode, MMU &mem)
{
	using namespace emath;

	// Setup

	int cycles = 0;
	bool done = false;
	CPUInstruction ins{};
	ins.origin = regs.pc;

	flags.byteToFlags(regs.f);
	regs.pc++;

	Logger::log(
			fmt::format("CPU: Executing instruction 0x{:02X}.", opcode),
			Logger::EXTREME
			);

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
		ins.opcode = 0xCB00 + opcode;
		Logger::log(
				fmt::format("CPU: Executing 2-Byte Instruction 0x{:04X}", ins.opcode),
				Logger::EXTREME
				);

		// ROTATE AND SHIFT //

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

		// RLC r - Shift register 'r' left, MSB to Carry flag
		if(!done && opcode >= 0b00000000 && opcode <= 0b00000111)
		{
			ins.mnemonic = "RLC";

			ins.target1 = toTarget(opcode & 0b00000111);

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				bool old_msb = (value >> 7) & 1;
				value = (value << 1) | old_msb;
				flags.carry = old_msb;
				setByteReg(ins.target1, value);

				flags.zero = (value == 0);

			} else {

				ins.t1_as_address = true;
				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				bool old_msb = (value >> 7) & 1;
				value = (value << 1) | old_msb;
				flags.carry = old_msb;

				mem.writeByte(regs.hl, value);
				cycles += 4;

				flags.zero = (value == 0);
			}

			flags.subtract = false;
			flags.half_carry = false;

			done = true;

		} // END RLC r

		// RL r - Shift register 'r' left, wrapped and set carry.
		if(!done && opcode >= 0b00010000 && opcode <= 0b00010111)
		{
			ins.mnemonic = "RL";

			ins.target1 = toTarget(opcode & 0b00000111);

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				bool old_msb = (value >> 7) & 1;
				value = (value << 1) | flags.carry;
				flags.carry = old_msb;
				setByteReg(ins.target1, value);

				flags.zero = (value == 0);

			} else {

				ins.t1_as_address = true;
				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				bool old_msb = (value >> 7) & 1;
				value = (value << 1) | flags.carry;
				flags.carry = old_msb;

				mem.writeByte(regs.hl, value);
				cycles += 4;

				flags.zero = (value == 0);

			}

			flags.subtract = false;
			flags.half_carry = false;

			done = true;

		} // END RL r

		// RRC r - Shift register 'r' right, LSB to Carry flag
		if(!done && opcode >= 0b00001000 && opcode <= 0b00001111)
		{
			ins.mnemonic = "RRC";

			ins.target1 = toTarget(opcode & 0b00000111);

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				bool old_lsb = value & 1;
				value = (value >> 1) | (old_lsb << 7);
				flags.carry = old_lsb;
				setByteReg(ins.target1, value);

				flags.zero = (value == 0);

			} else {

				ins.t1_as_address = true;
				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				bool old_lsb = value & 1;
				value = (value >> 1) | (old_lsb << 7);
				flags.carry = old_lsb;

				mem.writeByte(regs.hl, value);
				cycles += 4;

				flags.zero = (value == 0);
			}

			flags.subtract = false;
			flags.half_carry = false;

			done = true;

		} // END RRC

		// RR r - Shift register 'r' right, wrapped and set carry.
		if(!done && opcode >= 0b00011000 && opcode <= 0b00011111)
		{
			ins.mnemonic = "RR";

			ins.target1 = toTarget(opcode & 0b00000111);

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				bool old_lsb = value & 1;
				value = (value >> 1) | (flags.carry << 7);
				flags.carry = old_lsb;
				setByteReg(ins.target1, value);

				flags.zero = (value == 0);

			} else {

				ins.t1_as_address = true;
				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				bool old_lsb = value & 1;
				value = (value >> 1) | (flags.carry << 7);
				flags.carry = old_lsb;

				mem.writeByte(regs.hl, value);
				cycles += 4;

				flags.zero = (value == 0);
			}

			flags.subtract = false;
			flags.half_carry = false;

			done = true;

		} // END RR

		// SLA r - Shift r left into carry, set LSB to 0
		if(!done && opcode >= 0b00100000 && opcode <= 0b01000111)
		{
			ins.mnemonic = "SLA";

			ins.target1 = toTarget(opcode & 0b00000111);

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				bool old_msb = (value >> 7) & 1;
				value = value << 1;
				flags.carry = old_msb;
				setByteReg(ins.target1, value);

				flags.zero = (value == 0);

			} else {

				ins.t1_as_address = true;
				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				bool old_msb = (value >> 7) & 1;
				value = value << 1;
				flags.carry = old_msb;

				mem.writeByte(regs.hl, value);
				cycles += 4;

				flags.zero = (value == 0);
			}

			flags.subtract = false;
			flags.half_carry = false;

			done = true;

		} // END SLA

		// SRA r - Shift r right into Carry, leave MSB as-is
		if(opcode >= 0b00101000 && opcode <= 0b00101111)
		{
			ins.mnemonic = "SRA";

			ins.target1 = toTarget(opcode & 0b00000111);

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				bool old_lsb = value & 1;
				bool old_msb = (value >> 7) & 1;
				value = (value >> 1) | (old_msb << 7);
				flags.carry = old_lsb;
				setByteReg(ins.target1, value);

				flags.zero = (value == 0);

			} else {

				ins.t1_as_address = true;
				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				bool old_lsb = value & 1;
				bool old_msb = (value >> 7) & 1;
				value = (value >> 1) | (old_msb << 7);
				flags.carry = old_lsb;

				mem.writeByte(regs.hl, value);
				cycles += 4;

				flags.zero = (value == 0);

			}

			flags.subtract = false;
			flags.half_carry = false;

			done = true;

		} // END SRA

		// SRL r - Shift r left into Carry, MSB set to 0
		if(!done && opcode >= 0b00111000 && opcode <= 0b00111111)
		{
			ins.mnemonic = "SRL";

			ins.target1 = toTarget(opcode & 0b00000111);

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				bool old_lsb = value & 1;
				value = value >> 1;
				flags.carry = old_lsb;
				setByteReg(ins.target1, value);

				flags.zero = (value == 0);

			} else {

				ins.t1_as_address = true;
				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				bool old_lsb = value & 1;
				value = value >> 1;
				flags.carry = old_lsb;

				mem.writeByte(regs.hl, value);
				cycles += 4;

				flags.zero = (value == 0);

			}

			flags.subtract = false;
			flags.half_carry = false;

			done = true;

		} // END SRL

		// END ROTATE AND SHIFT //

		// SINGLE-BIT //

		// BIT b,r - Check bit 'b' in register 'r'
		if(!done && opcode >= 0b01000000 && opcode <= 0b01111111)
		{
			ins.mnemonic = "BIT";

			ins.target1 = toTarget(opcode & 0b00000111);
			// Bit to check is direct value in middle 3 bits
			uint8_t check_bit = ((opcode & 0b00111000) >> 3);

			if(ins.target1 != HL)
			{
				// Shift value so check_bit is at position 0,
				// then mask to only bit zero
				flags.zero = (getByteReg(ins.target1) >> check_bit) & 1;

			} else {

				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;
				flags.zero = (value >> check_bit) & 1;
			}

			flags.subtract = false;
			flags.half_carry = true;

			done = true;

		} // END BIT

		// RES b,r - Reset bit 'b' in register 'r'
		if(!done && opcode >= 0b10000000 && opcode <= 0b10111111)
		{
			ins.mnemonic = "RES";

			ins.target1 = toTarget(opcode & 0b00000111);
			// Bit to check is direct value in middle 3 bits
			uint8_t check_bit = (opcode & 0b00111000) >> 3;

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);

				// Create a mask of all ones except check_bit
				value &= ~(1 << check_bit);

				setByteReg(ins.target1, value);

			} else {

				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				// Create a mask of all ones except check_bit
				value &= ~(1 << check_bit);

				mem.writeByte(regs.hl, value);
				cycles += 4;
			}

			done = true;

		} // END RES

		// SET b,r - Set bit 'b' in register 'r'
		if(!done && opcode >= 0b11000000 && opcode <= 0b11111111)
		{
			ins.mnemonic = "SET";

			ins.target1 = toTarget(opcode & 0b00000111);
			// Bit to check is direct value in middle 3 bits
			uint8_t check_bit = (opcode & 0b00111000) >> 3;

			if(ins.target1 != HL)
			{
				uint8_t value = getByteReg(ins.target1);
				value |= (1 << check_bit);
				setByteReg(ins.target1, value);

			} else {

				uint8_t value = mem.readByte(regs.hl);
				cycles += 4;

				value |= (1 << check_bit);

				mem.writeByte(regs.hl, value);
				cycles += 4;
			}

			done = true;

		} // END SET

		// END SINGLE BIT //
	}

	// End Two-Byte instructions //

	// THE BIG SWITCH
	if (!done)
	switch(opcode)
	{
	// NOP
	case 0x00:
	{
		ins.mnemonic = "NOP";
		done = true;
		break;
	} // END NOP

	// Load Instructions //

	// LD A,n - Put immediate value 'n' into register A
	case 0xF0:
	{
		ins.mnemonic = "LD";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		uint8_t value = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		regs.a = value;

		done = true;

		break;
	} // END LD A,n

	// LD (HL),n - Put immediate value 'n' into value at address HL
	case 0x36:
	{
		ins.mnemonic = "LD";
		ins.target1 = HL;
		ins.t1_as_address = true;
		ins.target2 = IMMEDIATE;

		uint8_t value = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;


		mem.writeByte(regs.hl, value);
		cycles += 4;

		done = true;

		break;
	} // END LD (HL),n

	// LD (rr),A - Put A into byte at address in register 'rr'
	case 0x02: case 0x12:
	{
		ins.mnemonic = "LD";
		ins.target2 = A;
		ins.t1_as_address = true;

		switch (opcode)
		{
			case 0x02: ins.target1 = BC; break;
			case 0x12: ins.target1 = DE; break;
			default: break;
		}

		uint16_t address = getShortReg(ins.target1);
		mem.writeByte(address, regs.a);
		cycles += 4;

		done = true;

		break;
	} // END LD (rr),A

	// LD (nn), A - Put A into byte at address in immediate 16-bit value
	case 0xEA:
	{
		ins.mnemonic = "LD";
		ins.target1 = IMMEDIATE;
		ins.t1_as_address = true;
		ins.target2 = A;

		uint8_t lsb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint8_t msb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint16_t address = emath::bytesToUShort(msb, lsb);
		mem.writeByte(address, regs.a);
		cycles += 4;

		done = true;

		break;
	} // END LD (nn),A

	// LD r,n - Put immediate value 'n' into register 'r'
	case 0x06: case 0x0E: case 0x16: case 0x1E: case 0x26: case 0x2E: case 0x3E:
	{
		ins.mnemonic = "LD";
		ins.target2 = IMMEDIATE;

		// Middle 3 bits denote register
		ins.target1 = toTarget((opcode & 0b00111000) >> 3);

		uint8_t val = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		setByteReg(ins.target1, val);

		done = true;

		break;
	} // END LD r,n

	// LDH (C),A - Put value in A in value at address $FF00 + C
	case 0xE2:
	{
		ins.mnemonic = "LDH";
		ins.target1 = C;
		ins.t1_as_address = true;
		ins.target2 = A;

		uint16_t address = 0xFF00 + regs.c;
		mem.writeByte(address, regs.a);
		cycles += 4;

		done = true;

		break;
	} // END LDH (C),A

	// LDH A,(C) - Put value at address $FF00 + C into A
	case 0xF2:
	{
		ins.mnemonic = "LDH";
		ins.target1 = A;
		ins.target2 = C;
		ins.t2_as_address = true;

		uint16_t address = 0xFF00 + regs.c;

		uint8_t val = mem.readByte(address);
		cycles += 4;
		regs.a = val;

		done = true;

		break;
	} // END LDH A,(C)

	// LDH (n),A - Put value in A into value at address $FF00 + immediate byte
	case 0xE0:
	{
		ins.mnemonic = "LDH";
		ins.target1 = IMMEDIATE;
		ins.t1_as_address = true;
		ins.target2 = A;

		uint16_t address = 0xFF00;
		address += mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		mem.writeByte(address, regs.a);
		cycles += 4;

		done = true;

		break;
	} // END LDH (n),A

	// LD rr,nn - Load 16-bit immediate value into 16-bit register
	case 0x01: case 0x11: case 0x21: case 0x31:
	{
		ins.mnemonic = "LD";
		ins.target2 = IMMEDIATE;

		// Get target1
		switch (opcode)
		{
			case 0x01: ins.target1 = BC; break;
			case 0x11: ins.target1 = DE; break;
			case 0x21: ins.target1 = HL; break;
			case 0x31: ins.target1 = SP; break;
			default: break;
		}

		uint8_t lsb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint8_t msb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint16_t val = emath::bytesToUShort(msb, lsb);

		setShortReg(ins.target1, val);

		done = true;

		break;
	} // END LD rr,nn

	// LD SP,HL - Put HL into SP
	case 0xF9:
	{
		ins.mnemonic = "LD";
		ins.target1 = SP;
		ins.target2 = HL;

		regs.sp = regs.hl;

		done = true;

		break;
	} // END LD SP,HL

	// LD HL,SP+n - "Put SP + n effective address into HL" (SP + N) -> HL
	case 0xF8:
	{
		ins.mnemonic = "LD";
		ins.target1 = HL;
		ins.target2 = SP;

		uint16_t val1 = regs.sp;
		uint16_t val2 = mem.readByte(regs.pc);
		uint16_t sum = val1 + val2;
		regs.pc++;
		cycles += 4;

		regs.hl = sum;

		flags.zero = false;
		flags.subtract = false;
		flags.half_carry = emath::checkHCAdd(val1, val2);
		flags.carry = emath::checkOFAdd(val1, val2);

		done = true;

		break;
	} // END LD HL,SP+n

	// LD (nn),SP - Put SP into the value at address given by 16-bit immediate value
	case 0x08:
	{
		ins.mnemonic = "LD";
		ins.target1 = IMMEDIATE;
		ins.t1_as_address = true;
		ins.target2 = SP;

		// Get address from immediate data
		uint8_t address_lsb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;
		uint8_t address_msb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint16_t address = emath::bytesToUShort(address_msb, address_lsb);

		// Split SP into two bytes
		uint8_t value_lsb = 0, value_msb = 0;
		emath::ushortToBytes(regs.sp, &value_msb, &value_lsb);

		// Write split SP to memory
		mem.writeByte(address, value_lsb);
		address++;
		cycles += 4;
		mem.writeByte(address, value_msb);
		cycles += 4;

		done = true;

		break;
	} // END LD (nn),SP

	// LDI (HL),A - Load into address at $HL, A. Increment HL
	case 0x22:
	{
		ins.mnemonic = "LDI";
		ins.target1 = HL;
		ins.t1_as_address = true;
		ins.target2 = A;

		mem.writeByte(regs.hl, regs.a);
		cycles += 4;

		regs.hl++;

		done = true;

		break;
	} // END LDI (HL),A

	// LDI A,(HL) - Load into A, value at $HL. Increment HL
	case 0x2A:
	{
		ins.mnemonic = "LDI";
		ins.target1 = A;
		ins.target2 = HL;
		ins.t2_as_address = true;

		uint8_t val = mem.readByte(regs.hl);
		cycles += 4;
		regs.a = val;

		regs.hl++;

		done = true;

		break;
	} // END LDI (HL),A

	// LDD (HL), A - Load into A, value at $HL. Decrement HL.
	case 0x32:
	{
		ins.mnemonic = "LDD";
		ins.target1 = HL;
		ins.t1_as_address = true;
		ins.target2 = A;

		mem.writeByte(regs.hl, regs.a);
		cycles += 4;

		regs.hl--;

		done = true;

		break;
	} // END LDI (HL),A

	// LDI A,(HL) - Load into A, value at $HL. Decrement HL.
	case 0x3A:
	{
		ins.mnemonic = "LDD";
		ins.target1 = A;
		ins.target2 = HL;
		ins.t2_as_address = true;

		uint8_t val = mem.readByte(regs.hl);
		cycles += 4;
		regs.a = val;

		regs.hl--;

		done = true;

		break;
	} // END LDI (HL),A

	// PUSH - Push 16-bit register onto stack, decrement SP twice
	case 0xF5: case 0xC5: case 0xD5: case 0xE5:
	{
		ins.mnemonic = "PUSH";
		ins.target1 = SP;
		ins.t1_as_address = true;

		// MSB
		switch (opcode)
		{
			case 0xF5: ins.target2 = A; break;
			case 0xC5: ins.target2 = B; break;
			case 0xD5: ins.target2 = D; break;
			case 0xE5: ins.target2 = H; break;
			default: break;
		}

		regs.sp--;
		uint8_t msb = getByteReg(ins.target2);
		mem.writeByte(regs.sp, msb);
		cycles += 4;

		// LSB
		switch (opcode)
		{
			case 0xF5: ins.target2 = F; break;
			case 0xC5: ins.target2 = C; break;
			case 0xD5: ins.target2 = E; break;
			case 0xE5: ins.target2 = L; break;
			default: break;
		}

		regs.sp--;
		uint8_t lsb = getByteReg(ins.target2);
		mem.writeByte(regs.sp, lsb);
		cycles += 4;

		done = true;

		break;
	} // END PUSH

	// POP - Pop 16-bit value off of stack into 16-bit register, increment SP twice
	case 0xF1: case 0xC1: case 0xD1: case 0xE1:
	{
		ins.mnemonic = "POP";
		ins.target2 = SP;
		ins.t2_as_address = true;

		// LSB
		switch (opcode)
		{
			case 0xF1: ins.target1 = F; break;
			case 0xC1: ins.target1 = C; break;
			case 0xD1: ins.target1 = E; break;
			case 0xE1: ins.target1 = L; break;
			default: break;
		}

		regs.sp++;
		uint8_t val = mem.readByte(regs.sp);
		cycles += 4;

		setByteReg(ins.target1, val);

		// MSB
		switch (opcode)
		{
			case 0xF1: ins.target1 = A; break;
			case 0xC1: ins.target1 = B; break;
			case 0xD1: ins.target1 = D; break;
			case 0xE1: ins.target1 = H; break;
			default: break;
		}

		regs.sp++;
		val = mem.readByte(regs.sp);
		cycles += 4;

		setByteReg(ins.target1, val);

		// Update flags in case AF was loaded
		regs.f = flags.flagsToByte();

		done = true;

		break;

	} // END POP

	// End Load Instructions //



	// Arithmetic Instructions //

	// ADD A,n
	case 0xC6:
	{
		ins.mnemonic = "ADD";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		regs.pc++;
		uint8_t val1 = mem.readByte(regs.pc);
		cycles += 4;

		uint8_t val2 = regs.a;

		uint8_t sum = val1 + val2;

		regs.a = sum;

		flags.zero = (regs.a == 0);
		flags.subtract = false;
		flags.half_carry = emath::checkHCAdd(val1, val2);
		flags.carry = emath::checkOFAdd(val1, val2);

		done = true;

		break;
	} // END ADD A,n

	// CP A,n - Compares A with value in register 'r'. (SUB without changing A)
	case 0xFE:
	{
		ins.mnemonic = "CP";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		regs.pc++;
		uint8_t val = mem.readByte(regs.pc);
		cycles += 4;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = emath::checkHCSub(regs.a, val);
		flags.carry = emath::checkUFSub(regs.a, val);

		done = true;

		break;
	} // END CP A,n

	// ADD A,n - Add (immediate value 'n' + carry flag) to A
	case 0xCE:
	{
		ins.mnemonic = "ADC";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		uint8_t val1 = regs.a;
		uint8_t val2 = mem.readByte(regs.pc) + flags.carry;
		regs.pc++;
		cycles += 4;

		uint8_t sum = val1 + val2;

		regs.a = sum;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = emath::checkHCAdd(val1, val2);
		flags.carry = emath::checkOFAdd(val1, val2);

		done = true;

		break;
	} // END ADC A,n

	// SBC A,n - Subtract (immediate value 'n' + carry flag) from A
	case 0xDE:
	{
		ins.mnemonic = "SBC";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		uint8_t val1 = regs.a;
		uint8_t val2 = mem.readByte(regs.pc) + flags.carry;
		regs.pc++;
		cycles += 4;

		uint8_t dif = val1 - val2;

		regs.a = dif;

		flags.zero = (regs.a == 0);
		flags.subtract = true;
		flags.half_carry = emath::checkHCSub(val1, val2);
		flags.carry = emath::checkUFSub(val1, val2);

		done = true;

		break;
	} // END SBC A,n

	// AND A,n
	case 0xE6:
	{
		ins.mnemonic = "AND";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		uint8_t val = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		regs.a &= val;

		flags.zero = (regs.a == 0);
		flags.subtract = false;
		flags.half_carry = true;
		flags.carry = false;

		done = true;

		break;
	} // END ADD A,n

	// OR A,n
	case 0xF6:
	{
		ins.mnemonic = "OR";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		uint8_t val = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		regs.a |= val;

		flags.zero = (regs.a == 0);
		flags.subtract = false;
		flags.half_carry = true;
		flags.carry = false;

		done = true;

		break;
	} // END OR A,n

	// XOR A,n
	case 0xEE:
	{
		ins.mnemonic = "XOR";
		ins.target1 = A;
		ins.target2 = IMMEDIATE;

		uint8_t val = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		regs.a ^= val;

		flags.zero = (regs.a == 0);
		flags.subtract = false;
		flags.half_carry = true;
		flags.carry = false;

		done = true;

		break;
	} // END XOR A,n

	// ADD HL,rr - To HL, add HL + 16-bit register
	case 0x09: case 0x19: case 0x29: case 0x39:
	{
		ins.mnemonic = "ADD";

		switch(opcode)
		{
		case 0x09: ins.target2 = BC; break;
		case 0x19: ins.target2 = DE; break;
		case 0x29: ins.target2 = HL; break;
		case 0x39: ins.target2 = SP; break;
		default: break;
		}

		uint16_t val1 = regs.hl;
		uint16_t val2 = getShortReg(ins.target2);

		uint16_t sum = val1 + val2;

		regs.hl = sum;

		// Flags are not set

		done = true;

		break;
	} // END ADD HL,rr

	// ADD SP,nn
	case 0xE8:
	{
		ins.mnemonic = "ADD";
		ins.target1 = SP;
		ins.target2 = IMMEDIATE;

		// Get Immediate value
		uint8_t lsb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;
		uint8_t msb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint16_t val1 = regs.sp;
		uint16_t val2 = emath::bytesToUShort(msb, lsb);

		uint16_t sum = val1 + val2;

		regs.sp = sum;

		flags.zero = false;
		flags.subtract = false;
		// TODO: See if this checks lower or upper bytes
		flags.half_carry = emath::checkHCAdd(val1 & 0xFF, val2 & 0xFF);
		flags.carry = emath::checkOFAdd(val1, val2);

		done = true;

		break;
	} // END ADD SP,nn

	// INC r - Increment value in/at register 'r'
	case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C: case 0x34: case 0x3C:
	{
		ins.mnemonic = "INC";

		// Middle 3 bits define Target 1
		ins.target1 = toTarget((opcode & 0b00111000) >> 3);

		if (ins.target1 != HL) {
			uint8_t sum = getByteReg(ins.target1);
			sum++;
			setByteReg(ins.target1, sum);

			flags.zero = (sum == 0);
			flags.subtract = false;
			flags.half_carry = emath::checkHCAdd(sum, 1);

		} else {

			ins.t1_as_address = true;
			uint8_t sum = mem.readByte(regs.hl);
			cycles += 4;

			sum++;

			mem.writeByte(regs.hl, sum);
			cycles += 4;

			flags.zero = (sum == 0);
			flags.subtract = false;
			flags.half_carry = emath::checkHCAdd(sum, 1);
			// Carry is unchanged
		}

		done = true;

		break;
	} // END INC r

	// INC rr - Increment value in register 'rr'
	case 0x03: case 0x13: case 0x23: case 0x33:
	{
		ins.mnemonic = "INC";

		switch (opcode)
		{
			case 0x03: ins.target1 = BC; break;
			case 0x13: ins.target1 = DE; break;
			case 0x23: ins.target1 = HL; break;
			case 0x33: ins.target1 = SP; break;
			default: break;
		}

		uint16_t sum = getShortReg(ins.target1);
		sum++;
		setShortReg(ins.target1, sum);

		// Flags are not set

		done = true;

		break;
	} // END INC rr

	// DEC r - Decrement value in/at register 'r'
	case 0x05: case 0x0D: case 0x15: case 0x1D:
	case 0x25: case 0x2D: case 0x35: case 0x3D:
	{
		ins.mnemonic = "DEC";

		// Middle 3 bits define Target 1
		ins.target1 = toTarget((opcode & 0b00111000) >> 3);

		if (ins.target1 != HL) {
			uint8_t dif = getByteReg(ins.target1);
			dif--;
			setByteReg(ins.target1, dif);

			flags.zero = (dif == 0);
			flags.subtract = true;
			flags.half_carry = emath::checkHCSub(dif, 1);

		} else {

			ins.t1_as_address = true;
			uint8_t dif = mem.readByte(regs.hl);
			cycles += 4;

			dif--;

			mem.writeByte(regs.hl, dif);
			cycles += 4;

			flags.zero = (dif == 0);
			flags.subtract = false;
			flags.half_carry = emath::checkHCSub(dif, 1);
			// Carry is unchanged
		}

		done = true;

		break;
	} // END DEC r

	// DEC rr
	case 0x0B: case 0x1B: case 0x2B: case 0x3B:
	{
		ins.mnemonic = "DEC";

		switch (opcode)
		{
			case 0x0B: ins.target1 = BC; break;
			case 0x1B: ins.target1 = DE; break;
			case 0x2B: ins.target1 = HL; break;
			case 0x3B: ins.target1 = SP; break;
			default: break;
		}

		uint16_t dif = getShortReg(ins.target1);
		dif--;
		setShortReg(ins.target1, dif);

		// Flags are not set

		done = true;

		break;
	} // END DEC rr

	//DAA - Retroactively adjusts A to a valid BCD result. This means something, and does something.
	case 0x27:
	{
		ins.mnemonic = "DAA";
		ins.target1 = A;

		// Taken from user AWJ @ https://forums.nesdev.org/viewtopic.php?t=15944
		if (!flags.zero)
		{  // after an addition, adjust if (half-)carry occurred or if result is out of bounds
			if (flags.carry || regs.a > 0x99) { regs.a += 0x60; flags.carry = true; }
			if (flags.half_carry || (regs.a & 0x0f) > 0x09) { regs.a += 0x6; }
		} else
		{  // after a subtraction, only adjust if (half-)carry occurred
			if (flags.carry) { regs.a -= 0x60; }
			if (flags.half_carry) { regs.a -= 0x60; }
		}
		// these flags are always updated
		flags.zero = (regs.a == 0); // the usual z flag
		flags.half_carry = false; // h flag is always cleared

		done = true;

		break;
	} // END DAA

	// CPL - Flip all bits in A
	case 0x2F:
	{
		ins.mnemonic = "CPL";
		ins.target1 = A;

		regs.a = ~regs.a;

		flags.subtract = true;
		flags.half_carry = true;

		done = true;

		break;
	} // END CPL

	// End Arithmetic Instructions //



	// Control Instructions //

	//CCF - Flip Carry flag
	case 0x3F:
	{
		ins.mnemonic = "CCF";
		ins.target1 = F;

		flags.carry = !flags.carry;

		done = true;

		break;
	} // END CCF

	//SCF - Set Carry flag
	case 0x37:
	{
		ins.mnemonic = "SCF";
		ins.target1 = F;

		flags.carry = true;

		done = true;

		break;
	} // END SCF

	// HALT
	case 0x76:
	{
		ins.mnemonic = "HALT";

		halted = true;

		done = true;

		break;
	} // END HALT

	// STOP
	case 0x10:
	{
		ins.mnemonic = "STOP";

		// TODO: Put Emulator in STOPPED state instead of crashing it.
		throw std::runtime_error("STOP CALLED");

		done = true;

		break;
	} // END STOP

	// DI - Disable Interrupts after next instruction is executed
	case 0xF3:
	{
		ins.mnemonic = "DI";
		next_interrupt_state = false;

		done = true;

		break;
	} // END DI

	// EI - Enable Interrupts after next instruction is executed
	case 0xFB:
	{
		ins.mnemonic = "EI";
		next_interrupt_state = true;

		done = true;

		break;
	} // END EI

	// End Control Instructions //

	// JP nn,c - If condition met, jump to the immediate value 'nn'
	case 0xC3: case 0xC2: case 0xCA: case 0xD2: case 0xDA:
	{
		ins.mnemonic = "JP";
		ins.target1 = IMMEDIATE;

		bool condition_met = true;

		switch (opcode)
		{
			case 0xC2: condition_met = !flags.zero; break;
			case 0xCA: condition_met = flags.zero; break;
			case 0xD2: condition_met = !flags.carry; break;
			case 0xDA: condition_met = flags.carry; break;
			default: break;
		}

		uint8_t lsb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;
		uint8_t msb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint16_t  address = emath::bytesToUShort(msb, lsb);

		if (condition_met) { regs.pc = address; }

		done = true;

		break;
	} // END JP nn,c

	// JR n,c - If condition met, jump to PC + 'n'
	case 0x18: case 0x20: case 0x28: case 0x30: case 0x38:
	{
		ins.mnemonic = "JR";
		ins.target1 = IMMEDIATE;

		bool condition_met = true;

		switch (opcode)
		{
			case 0x20: condition_met = !flags.zero; break;
			case 0x28: condition_met = flags.zero; break;
			case 0x30: condition_met = !flags.carry; break;
			case 0x38: condition_met = flags.carry; break;
			default: break;
		}

		uint16_t address = regs.pc;

		address += mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		if (condition_met) { regs.pc = address; }

		done = true;

		break;
	} // END JR n,c

	// CALL - Push current PC onto stack, jump to address in immediate 16 bits
	case 0xCD: case 0xC4: case 0xCC: case 0xD4: case 0xDC:
	{
		ins.mnemonic = "CALL";
		ins.target1 = IMMEDIATE;

		bool condition_met = true;

		switch(opcode)
		{
			case 0x20: condition_met = !flags.zero; break;
			case 0x28: condition_met = flags.zero; break;
			case 0x30: condition_met = !flags.carry; break;
			case 0x38: condition_met = flags.carry; break;
			default: break;
		}

		// Get address from nn
		uint8_t lsb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;
		uint8_t msb = mem.readByte(regs.pc);
		regs.pc++;
		cycles += 4;

		uint16_t jump_address = emath::bytesToUShort(msb, lsb);

		if(condition_met)
		{
			// Break PC into bytes
			uint8_t pclsb = 0, pcmsb = 0;
			emath::ushortToBytes(regs.pc, &pclsb, &pcmsb);

			// Push MSB
			regs.sp--;
			mem.writeByte(regs.sp, pcmsb);
			cycles += 4;

			// Push LSB
			regs.sp--;
			mem.writeByte(regs.sp, pclsb);
			cycles += 4;

			// Jump to address
			regs.pc = jump_address;
		}

		done = true;

		break;
	} // END CALL

	// RET - Pop from stack and jump to that address
	case 0xC9: case 0xC0: case 0xC8: case 0xD0: case 0xD8:
	{
		ins.mnemonic = "RET";
		ins.target1 = IMMEDIATE;

		bool condition_met = true;

		switch(opcode)
		{
			case 0xC0: condition_met = !flags.zero; break;
			case 0xC8: condition_met = flags.zero; break;
			case 0xD0: condition_met = !flags.carry; break;
			case 0xD8: condition_met = flags.carry; break;
			default: break;
		}

		if(condition_met)
		{
			// POP address
			regs.sp++;
			uint8_t lsb = mem.readByte(regs.sp);
			cycles += 4;

			regs.sp++;
			uint8_t msb = mem.readByte(regs.sp);
			cycles += 4;

			uint16_t value = emath::bytesToUShort(msb, lsb);

			// Jump to address
			regs.pc = value;
		}

		done = true;

		break;
	} // END RET

	// RETI - Pop from stack and jump to that address, then enable interrupts
	case 0xD9:
	{
		ins.mnemonic = "RET";
		ins.target1 = IMMEDIATE;

		// POP address
		regs.sp++;
		uint8_t lsb = mem.readByte(regs.sp);
		cycles += 4;

		regs.sp++;
		uint8_t msb = mem.readByte(regs.sp);
		cycles += 4;

		uint16_t value = emath::bytesToUShort(msb, lsb);

		next_interrupt_state = true;

		// Jump to address
		regs.pc = value;

		done = true;

		break;

	} // END RETI

	// RST n - Push current address onto stack, jump to vector
	case 0xC7: case 0xCF: case 0xD7: case 0xDF: case 0xE7: case 0xEF: case 0xF7: case 0xFF:
	{
		ins.mnemonic = "RST";

		uint16_t vector = 0;

		// Get jump location
		switch(opcode)
		{
			case 0xC7: vector = 0x00; break;
			case 0xCF: vector = 0x08; break;
			case 0xD7: vector = 0x10; break;
			case 0xDF: vector = 0x18; break;
			case 0xE7: vector = 0x20; break;
			case 0xEF: vector = 0x28; break;
			case 0xF7: vector = 0x30; break;
			case 0xFF: vector = 0x38; break;
			default: break;
		} // END SWITCH

		// Break PC into bytes
		uint8_t lsb = 0, msb = 0;
		emath::ushortToBytes(regs.pc, &lsb, &msb);

		// Push MSB
		regs.sp--;
		mem.writeByte(regs.sp, msb);
		cycles += 4;

		// Push LSB
		regs.sp--;
		mem.writeByte(regs.sp, lsb);
		cycles += 4;

		// Jump to address
		regs.pc = vector;

		done = true;

		break;

	} // END RST n

	// End Jump Instructions //



	// End Rotate and Shift Instructions //

	// RLCA - Shift A left - MSB to Carry flag and LSB
	case 0x07:
	{
		ins.mnemonic = "RLCA";
		ins.target1 = A;

		uint8_t value = regs.a;

		bool old_msb = (value >> 7) & 1;
		value = (value << 1) | old_msb;
		flags.carry = old_msb;
		regs.a = value;

		flags.zero = false;
		flags.subtract = false;
		flags.half_carry = false;

		done = true;

		break;
	} // END RLCA

	// RRCA - Shift A right - old LSB to carry flag and MSB
	case 0x0F:
	{
		ins.mnemonic = "RRCA";
		ins.target1 = A;

		uint8_t value = regs.a;

		bool old_lsb = value & 1;
		value = (value >> 1) | (old_lsb << 7);
		flags.carry = old_lsb;
		regs.a = value;

		flags.zero = false;
		flags.subtract = false;
		flags.half_carry = false;

		done = true;

		break;
	} // END RRCA

	// RLA - Shift A left - LSB to Carry flag, MSB to old Carry flag
	case 0x17:
	{
		ins.mnemonic = "RLA";
		ins.target1 = A;

		uint8_t value = regs.a;

		bool old_msb = (value >> 7) & 1;
		value = (value << 1) | flags.carry;
		flags.carry = old_msb;
		regs.a = value;

		flags.zero = false;
		flags.subtract = false;
		flags.half_carry = false;

		done = true;

		break;
	} // END RLA

	// RRA - Shift A right - LSB to Carry flag, MSB to old Carry flag
	case 0x1F:
	{
		ins.mnemonic = "RRA";
		ins.target1 = A;

		uint8_t value = regs.a;

		bool old_lsb = value & 1;
		value = (value >> 1) | (flags.carry << 7);
		flags.carry = old_lsb;
		regs.a = value;

		flags.zero = false;
		flags.subtract = false;
		flags.half_carry = false;

		done = true;

		break;
	} // END RRA

	// End Rotate and Shift Instructions //


	// End Jump Instructions //

	default: break;
	} // END THE BIG SWITCH


	// TODO: The rest of the instructions



	// Final logging/cleanup

	if(!done)
	{
		Logger::log(
				fmt::format("CPU: Unhandled instruction 0x{:02X}!",
									opcode),
				Logger::DEBUG
				);
	} else {

		Logger::log(
				fmt::format("CPU: Executed instruction {0}",
									instructionToString(ins)),
				Logger::EXTREME
				);
		Logger::log(
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
uint8_t CPU::getByteReg(TargetID target) const
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
		throw std::invalid_argument(
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
			throw std::invalid_argument(
					"Attempted write of invalid target in setByteReg()"
					);
		}
	}
}

// Gets a short from a 16-bit register
uint16_t CPU::getShortReg(TargetID target) const
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
		throw std::invalid_argument(
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
		throw std::invalid_argument(
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
			throw std::logic_error(
			"Invalid ID in toTarget(). If this happens, let Moon know."
			);
		}
	}
}