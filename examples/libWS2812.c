/**
 * @brief Control ws2812b led's
 * @author Michael Burmeister
 * @date October 31, 2020
 * @version 1.0
 * 
*/
#include "Custom/ws2812.h"
#include <stdio.h>
#include <propeller2.h>


#define WSLED 40
#define LED_COUNT 12


unsigned long ledColors[LED_COUNT];
int i;
unsigned long c[3];

int main()
{
  int x;
  
  printf("Starting\n");
  _dirh(57);
  _pinh(57);

  c[0] = COLOR(32,0,0);
  c[1] = COLOR(0,32,0);
  c[2] = COLOR(0,0,32);
  
  x = 0;
  
  for (int i=0;i<12;i++)
  {
    for (int j=0;j<LED_COUNT;j++)
    {
      if (x > 2)
        x = x - 3;
      ledColors[j] = c[x++];
    }
    x++;
    ws2812_set(WSLED, ledColors, LED_COUNT);
    _wait(1000);
  }
 
  for (int i=0;i<LED_COUNT;i++)
    ledColors[i] = 0;
  
  ws2812_set(WSLED, ledColors, LED_COUNT);
  
  printf("Done\n");
  
  while(1)
  {
    _wait(1000);
  }  
}
