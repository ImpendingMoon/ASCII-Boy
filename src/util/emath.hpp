/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/emath.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Has some extra math functions, such as wrap, byteToUShort, etc.
 ******************************************************************************/

#pragma once

#include <cstdint>

namespace emath
{
    // FUNCTIONS //

    // Converts two bytes into one uint16_t
    uint16_t bytesToUShort(uint8_t msb, uint8_t lsb);
    // Converts a uint16_t into two bytes
    void ushortToBytes(uint16_t value, uint8_t* msb, uint8_t* lsb);

	// Checks if addition of two bytes (a + b) results in an overflow
	bool checkOFAdd(uint8_t a, uint8_t b);
	// Checks if addition of two ushorts (a + b) results in an overflow
	bool checkOFAdd(uint16_t a, uint16_t b);

	// Checks if subtraction of two bytes (a - b) results in an overflow
	bool checkUFSub(uint8_t a, uint16_t b);
	// Checks if subtraction of two ushorts (a - b) results in an underflow
	bool checkUFSub(uint16_t a, uint16_t b);

	// Checks if addition of two bytes (a + b) results in a half-carry
	bool checkHCAdd(uint8_t a, uint8_t b);
	// Checks if subtraction of two bytes (a - b) results in a half-carry
	bool checkHCSub(uint8_t a, uint8_t b);

	// Wraps the value from the min/max
    int wrap(int value, int min, int max);
}
