/**
 * @brief HX8357 320x480 display panel
 * @author Michael Burmeister
 * @date March 11, 2019
 * @version 1.0
*/

#include "Custom/HX8357.h"
#include <propeller.h>
#include <stdio.h>

#define LITE 14
#define RST 13
#define DC 12
#define CS 11
#define MOSI 10
#define MISO 9
#define CLK 8

int i;
unsigned long PCNT;

int main()
{
  wait(1000);
  printf("Starting\n");
  i = HX8357_open(CLK, MISO, MOSI, CS, DC, RST, LITE);
  
  printf("Power: %x\n", i);

  if ((i & 0x08) == 0)
    HX8357_sleepOn(0);

  // Is display on
  if ((i & 0x04) == 0)
    HX8357_displayOn(1);
  
  i = HX8357_control();
  printf("Control: %x\n", i);

  HX8357_rotation(1);

  HX8357_cls(HX8357_BLACK);
  
  HX8357_writeSStr(10, 10, "Hello World");
  
  HX8357_writeStr(10, 110, "Hello World");
  
  HX8357_writeXStr(10, 160, "Hello World");
  
  HX8357_drawBox(50, 50, 100, 100, HX8357_color(255,0,0));
  
  HX8357_drawCircle(150, 75, 25, HX8357_BLUE);

  HX8357_displayMode(0);
  
  printf("Done\n");
  
  while(1)
  {
    wait(1000);
    
  }  
}
