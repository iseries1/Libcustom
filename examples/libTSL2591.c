/**
 * @brief Test Library Driver TSL2591 Light Sensor
 * @author Michael Burmeister
 * @date April 14, 2022
 * @version 1.0
 * 
*/

#include "Custom/TSL2591.h"
#include <stdio.h>
#include <propeller.h>

#define TSLSCL 37
#define TSLSDA 36

int i;

int main(int argc, char** argv)
{
    
    printf("Starting\n");

    i = TSL2591_Init(TSLSCL, TSLSDA);

    printf("ID: %d\n", i);
    
    i = TSL2591_Enable(0x03);

    printf("Enable: %x\n", i);

    i = TSL2591_Status();

    printf("Status: %x\n", i);

    i = TSL2591_ALS(0, 0);

    printf("Sensor Gain: %x\n", i);

    i = TSL2591_ReadALS(0);

    printf("Channel 0: %d\n", i);

    i = TSL2591_ReadALS(1);

    printf("Channel 1: %d\n", i);

    printf("Done\n");
    
    while (1)
    {
        printf("0: %d, 1: %d\n", TSL2591_ReadALS(0), TSL2591_ReadALS(1));
        wait(1000);
	}
}
