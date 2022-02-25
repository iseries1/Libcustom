/**
 * @brief Driver for e-Paper display
 * @author Michael Burmeister
 * @date July 7, 2019
 * @version 1.0
 * 
*/

#include <propeller2.h>
#include <stdio.h>
#include "Custom/epaper.h"


#define DIN 8
#define CLK 9
#define CS 10
#define DC 11
#define RST 12
#define BUSY 13

int i;

int main()
{
  printf("Starting\n");
  
  i = epaper_init(DIN, CLK, CS, DC, RST, BUSY);
  
//  printi("Value: %d\n", i);
  
  epaper_cls(1);
  
  unsigned char *x = epaper_buffer();

//  epaper_drawLine(10,10,50,50,0);
//  epaper_writeStr(8, 8, "Hello World");
  epaper_writeXStr(8, 64, "Hello");
  epaper_writeXStr(64, 64, "World");
//  epaper_writeChar(8, 32, 'A');
//  epaper_drawLine(120, 32, 100, 64, 0);
  epaper_update();
  printf("Done\n");
  epaper_sleep(1);
  
  while(1)
  {
    wait(1000);    
  }  
}
