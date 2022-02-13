/**
 * @brief Clock Calender device
 * @author Michael Burmeister
 * @date January 14, 2017
 * @version 1.0
 * 
*/

#include <stdio.h>
#include <propeller2.h>
#include <time.h>
#include "Custom/ds1302.h"


#define MISO 20
#define MOSI 21
#define SCLK 22
#define CS   23

time_t tm;
int m, s, d, mn, hr, yr, dw;


int main()
{
  tm = 0;
  
  DS1302_open(MISO, CS, SCLK, MOSI);
    
  _wait(1000);
  
  m = DS1302_getWriteProtect();
  if (m)
    printf("Write Protect On\n");
  else
    printf("Write Protect Off\n");
  
  if (0) //change to 1 to set
  {
    DS1302_clearWriteProtect();
    DS1302_setMessage("short message");
    DS1302_setWriteProtect();
  }
  
  if (0) //change to 1 to set
  {
    DS1302_clearWriteProtect();
    DS1302_setDate(20, 2, 21);
    DS1302_setTime(20, 44, 00);
    DS1302_setWriteProtect();
  }
  
  if (1)
  {
    m = DS1302_getMinutes();
    s = DS1302_getSeconds();
    d = DS1302_getDay();
    hr = DS1302_getHours();
    mn = DS1302_getMonth();
    dw = DS1302_getWeekDay();
    yr = 2000 + DS1302_getYear();
    
    printf("Date: %d/%d/%d, %d:%02d:%02d\n", mn, d, yr, hr, m, s);

    //printf("ticks: %d\n", time(&tm));
    DS1302_setDateTime();
    printf("Message: %s\n", DS1302_getMessage());
  }
  
  while (1)
  {
    time(&tm);
    printf("Date: %s", asctime(localtime(&tm)));
   _wait(10000);
  }
      
}
