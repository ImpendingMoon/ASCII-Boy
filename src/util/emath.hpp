/******************************************************************************
 PROJECT: ASCII-Boy
 PATH   : util/emath.hpp
 AUTHOR : ImpendingMoon
 EDITORS: ImpendingMoon,
 CREATED: 3 Dec 2022
 EDITED : 7 Dec 2022
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


    // Wraps the value from the min/max
    int wrap(int value, int min, int max);
}
