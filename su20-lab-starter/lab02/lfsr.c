#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg)
{
    /* YOUR CODE HERE */
    // 0000 0000 0010 1101
    uint16_t zero, two, three, five;
    uint16_t mask = 0x002D;
    uint16_t cur = mask & (*reg);
    if (cur & 0x0001)
    {
        zero = 1;
    }
    else
    {
        zero = 0;
    }
    if (cur & 0x0004)
    {
        two = 1;
    }
    else
    {
        two = 0;
    }
    if (cur & 0x0008)
    {
        three = 1;
    }
    else
    {
        three = 0;
    }
    if (cur & 0x0020)
    {
        five = 1;
    }
    else
    {
        five = 0;
    }
    int ext = zero ^ two ^ three ^ five;
    uint16_t sup;
    if (ext)
    {
        sup = 0x8000;
    }
    else
    {
        sup = 0x0000;
    }
    *reg = (*reg) >> 1;
    *reg = (*reg) & 0x7fff;
    (*reg) = (*reg) | sup;
}
