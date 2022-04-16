/**
 * @brief Test HB1750 Light Sensor
 * @author Michael Burmeister
 * @date April 15, 2022
 * @version 1.0
 * 
*/

#include "Custom/BH1750.h"
#include <stdio.h>
#include <propeller.h>

#define HBSCL 37
#define HBSDA 36


int i;


int main(int argc, char** argv)
{

    i = BH1750_Init(HBSCL, HBSDA);

    if (i < 0)
    {
        printf("Sensor Not Found\n");
        while (1)
            wait(500);
    }

    BH1750_Power(1);

    BH1750_Measure(0);

    wait(500);

    i = BH1750_GetMeasurement();

    printf("Value: %d\n", i);

    while (1)
    {
        BH1750_Measure(0);
        wait(1000);
        i = BH1750_GetMeasurement();
        printf("Value: %d\n", i);
	}
}
