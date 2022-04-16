/**
 * @brief Test ms8607 Temperature Humidity and Pressure sensor
 * @author Michael Burmeister
 * @date April 12, 2022
 * @version 1.0
 * 
*/

#include "Custom/ms8607.h"
#include <stdio.h>
#include <propeller.h>

#define msSCL 37
#define msSDA 36

int i;


int main(int argc, char** argv)
{
    
    ms8607_Init(msSCL, msSDA);

    i = ms8607_Reset();

    if (i != 0)
    {
        printf("Sensor Not Found\n");
        while (1)
            wait(500);
    }

    i = ms8607_Calibration();

    printf("Factory: %x\n", i);

    i = ms8607_GetTemperature(4);

    printf("Temperature: %d, %d\n", i, i * 9/5 + 3200);
    
    i = ms8607_GetPressure(4);

    printf("Pressure: %d (%d)\n", i, i * 100 / 3386);

    i = ms8607_SetUserRegister(0);

    printf("Humidity User: %x\n", i);

    i = ms8607_GetHumidity();

    printf("Humidity: %d\n", i);

    while (1)
    {
        wait(1000);
	}
}
