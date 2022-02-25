/**
 * @brief INA260 Adafruit power driver
 * @author Michael Burmeister
 * @date June 23, 2019
 * @version 1.0
 * 
*/

#include <propeller.h>
#include <stdio.h>
#include "Custom/ina260.h"


#define CLK 36
#define DTA 37

int i;

int main()
{
  i = INA260_open(CLK, DTA);
  
  printf("MFG: %x\n", i);
  
  i = INA260_getVoltage();
  
  printf("Volts: %d\n", i);
  
  i = INA260_getCurrent();
  
  printf("Current: %d\n", i);
  
  i = INA260_getPower();
  
  printf("Power: %d\n", i);
  
  INA260_setConfig(INA260_BOTH_CONTINUOUS, INA260_1100, INA260_1100, INA260_AVG1024, 0);
  
  i = INA260_getConfig();
  
  printf("Config: %x\n", i);
  
  while(1)
  {
    wait(1000);
    i = INA260_getCurrent();
    printf("Current: %d  ", i);
    i = INA260_getVoltage();
    printf("Voltage: %d\n", i);
  }  
}
