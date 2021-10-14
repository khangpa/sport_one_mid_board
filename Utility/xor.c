/*
 *  xor.c
 *
 *  Created on: Dec 25, 2020
 *      Author: Truong VV
 */

#include "xor.h"

/******************************************************************************/
/*                              FUNCTION                                      */
/******************************************************************************/

u8 XOR_Calculator(uint8_t* data, uint8_t offset, uint16_t length)
{
    u8 result = 0;
    for(u8 i = offset; i < length; i++)
    {
        result ^= data[i];
    }
    return result;
}