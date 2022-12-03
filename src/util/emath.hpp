/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/emath.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 3 Dec 2022
 ******************************************************************************/

/******************************************************************************
 Has some extra math functions, such as wrap, byteToUShort, etc.
 ******************************************************************************/

#pragma once

// Typedefs for easier to remember types
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;

namespace emath
{
    // FUNCTIONS //

    // Converts two bytes into one ushort
    ushort bytesToUShort(byte msb, byte lsb);
    // Converts a ushort into two bytes
    void ushortToBytes(ushort value, byte* msb, byte* lsb);


    // Wraps the value from the min/max
    int wrap(int value, int min, int max);
}