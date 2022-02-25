/**
 * @brief Driver for DS3231 (HW084)
 * @author Michael Burmeister
 * @date December 25, 2020
 * @version 1.0
 * 
*/

#include <stdio.h>
#include <propeller2.h>
#include <time.h>
#include "Custom/DS3231.h"

#define DSCLK 37
#define DSSDA 36

time_t tm;

int main()
{
    int i;
    
    printf("Starting...\n");

    DS3231_Open(DSCLK, DSSDA);
    
//    DS3231_SetDate(12,8,20);
//    DS3231_SetTime(9,33,0);
    
	DS3231_SetDateTime();
		
    while (1)
    {
    	time(&tm);
    	i = DS3231_Temperature();
    	printf("Temp %d, Date: %s", i, asctime(localtime(&tm)));
        wait(1000);
	}
}
