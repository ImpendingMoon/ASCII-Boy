/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/emath.cpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 3 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Has some extra math functions, such as clamp, wrap, byteToShort, etc.
 ******************************************************************************/

#include "emath.hpp"

// Converts two bytes into one ushort
ushort emath::bytesToUShort(byte msb, byte lsb)
{
    ushort output;
    output = (msb << 8);
    output += lsb;

    return output;
}


// Converts a ushort into two bytes
void emath::ushortToBytes(ushort value, byte* msb, byte* lsb)
{
    *msb = (value & 0xFF00) >> 8;
    *lsb = (value & 0x00FF);
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