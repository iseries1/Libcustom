/**
 * @brief Driver for ws2812 leds
 * @author Michael Burmeister
 * @date November 10, 2020
 * @version 1.0
 * 
*/

#include <propeller2.h>
#include <stdio.h>
#include "ws2812.h"

void ws2812_set(int pin, unsigned long *leds, int num)
{
    unsigned long color;
    unsigned int w;
    int bits;
    short H, L;

    H = _clkfreq/1111111; // 90nsec (1250000 80ns)
    L = _clkfreq/2857143; // 35nsec (2500000 40ns)

    _dirh(pin);
    _pinl(pin);

    for (int i=0;i<num;i++)
    {
    	color = leds[i];
    	bits = 24;

      asm volatile(" shl %[_color], #8\n"
        "L1: rcl %[_color], #1 wc\n"
        "		drvh %[_pin]\n"
        "if_c waitx  %[_H]\n"
         "if_nc  waitx  %[_L]\n"
         "		drvl %[_pin]\n"
         "if_c   waitx  %[_L]\n"
         "if_nc  waitx  %[_H]\n"
         "		djnz %[_bits], #L1\n"
         :[_color] "+r"(color),[_bits]"+r"(bits)
         :[_H]"r"(H), [_L]"r"(L), [_pin]"r"(pin)
      );
    }
}
