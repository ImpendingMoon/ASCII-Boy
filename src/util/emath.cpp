/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/emath.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 11 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Has some extra math functions, such as clamp, wrap, byteToShort, etc.
 ******************************************************************************/

#include "emath.hpp"

// Converts two bytes into one uint16_t
uint16_t emath::bytesToUShort(uint8_t msb, uint8_t lsb)
{
    uint16_t output;
    output = (msb << 8);
    output += lsb;

    return output;
}


// Converts a uint16_t into two bytes
void emath::ushortToBytes(uint16_t value, uint8_t* msb, uint8_t* lsb)
{
    *msb = (value & 0xFF00) >> 8;
    *lsb = (value & 0x00FF);
}



// Checks if addition of two bytes (a + b) results in an overflow
bool emath::checkOFAdd(uint8_t a, uint8_t b)
{
	return ( a > (UINT8_MAX - b) );
}

// Checks if addition of two ushorts (a + b) results in an overflow
bool emath::checkOFAdd(uint16_t a, uint16_t b)
{
	return ( a > (UINT16_MAX - b) );
}


// Checks if subtraction of two bytes (a - b) results in an overflow
bool emath::checkUFSub(uint8_t a, uint16_t b)
{
	return ( b > a );
}

// Checks if subtraction of two ushorts (a - b) results in an underflow
bool emath::checkUFSub(uint16_t a, uint16_t b)
{
	return ( b > a );
}


// Checks if addition of two bytes (a + b) results in a half-carry
bool emath::checkHCAdd(uint8_t a, uint8_t b)
{
	// If both lower nibbles add up to over 0b00001111, half-carried
	return ( ((a & 0xF) + (b & 0xF)) > 0xF );
}

// Checks if subtraction of two bytes (a - b) results in a half-carry
bool emath::checkHCSub(uint8_t a, uint8_t b)
{
	return ( (b & 0xF) > (a & 0xF) );
}



// Wraps the value from the min/max
int emath::wrap(int value, int min, int max)
{
    int output = value;

    while(output < min || output > max)
    {
        if(output > max) { output = min + (output - max); }
        if(output < min) { output = max - output; }
    }

    return output;
}
