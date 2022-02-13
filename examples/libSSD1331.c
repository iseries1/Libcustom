/**
 * @brief Colored SSD1331 Display Driver
 * @author Michael Burmeister
 * @date December 11, 2018
 * @version 1.0
 * 
*/

#include <propeller.h>
#include <stdio.h>
#include "Custom/SSD1331.h"


#define OLEDDIN 9
#define OLEDCLK 10
#define OLEDCS  11
#define OLEDDC  12
#define OLEDRST 13


int main()
{
  _wait(1000);
  printf("Starting\n");
  _wait(1000);
  
  SSD1331_init(OLEDDIN, OLEDCLK, OLEDCS, OLEDDC, OLEDRST);

  SSD1331_textColor(SSD1331_color(255, 255, 255), 0);
  
  SSD1331_writeSStr(15, 20, "Hello");

  SSD1331_drawLine(15, 50, 90, 50, SSD1331_color(255, 0, 0));
  
  SSD1331_drawBox(10, 15, 95, 55, SSD1331_color(0, 255, 0));
  
//  _wait(5000);
  
//  SSD1331_scrollRight(1, 0, 63, 1);
  
//  _wait(5000);
 
//  SSD1331_scrollStop();
  
  //print("Done\n");
  _pinl(56);
  
  while(1)
  {
      _wait(1000);
  }  
}
