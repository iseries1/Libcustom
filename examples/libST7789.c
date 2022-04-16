/**
 * @brief ST7789 240X320 display panel
 * @author Michael Burmeister
 * @date March 28, 2022
 * @version 1.0
*/

#include "Custom/st7789.h"
#include <propeller.h>
#include <stdio.h>

#define LITE 13
#define RST 12
#define DC 11
#define CS 10
#define MOSI 9
#define MISO 14
#define CLK 8

int i;
unsigned long PCNT;

int main()
{
  wait(1000);
  printf("Starting\n");
  ST7789_open(CLK, MISO, MOSI, CS, DC, RST, LITE);
  i = 0;

  pinh(13);
  printf("Power: %x\n", i);

  if ((i & 0x08) == 0)
    ST7789_sleepOn(0);

  // Is display on
  if ((i & 0x04) == 0)
    ST7789_displayOn(1);
  
//  i = ST7789_control();
//  printf("Control: %x\n", i);

  ST7789_rotation(1);

  ST7789_cls(ST7789_BLACK);
  
  ST7789_writeSStr(10, 10, "Hello World");
  
  ST7789_writeStr(10, 110, "Hello World");
  
  ST7789_writeXStr(10, 160, "Hello World");
  
  ST7789_drawBox(50, 50, 100, 100, ST7789_color(255,0,0));
  
  ST7789_drawCircle(150, 75, 25, ST7789_BLUE);

  //ST7789_displayMode(0);
  
  printf("Done\n");
  
  while(1)
  {
    wait(1000);
    
  }  
}
