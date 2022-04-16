/**
 * @brief TSL2591 Light Sensor Driver
 * @author Michael Burmeister
 * @date April 14, 2022
 * @version 1.0
 * 
*/

#define TSL2591_ADDRESS 0x52
#define TSL2591_CMD 0xa0
#define TSL2591_CTL 0x01
#define TSL2591_SPL 0xe0
#define TSL2591_ENABLE 0x00
#define TSL2591_ID 0x12
#define TSL2591_STATUS 0x13
#define TSL2591_CHN0 0x14
#define TSL2591_CHN1 0x16

int TSL2591_Read(int);
int TSL2591_Write(int);

#include "TSL2591.h"
#include "i2c.h"
#include <propeller.h>

i2c_t *tsl;


int TSL2591_Init(int scl, int sda)
{
    int i;

    tsl = I2C_Init(scl, sda, I2C_STD);

    i = TSL2591_Read(TSL2591_ID);

    return i;
}

int TSL2591_Enable(int value)
{
    int i;

    i = TSL2591_Read(TSL2591_ENABLE);
    TSL2591_Write(value);

    return i;
}

int TSL2591_Status(void)
{
    int i;

    i = TSL2591_Read(TSL2591_STATUS);

    return i;
}

int TSL2591_ReadALS(int Channel)
{
    int i;

    i = 0;

    if (Channel == 0)
    {
        i = TSL2591_Read(TSL2591_CHN0);
        i = i | (TSL2591_Read(TSL2591_CHN0+1) << 8);
    }
    if (Channel == 1)
    {
        i = TSL2591_Read(TSL2591_CHN1);
        i = i | (TSL2591_Read(TSL2591_CHN1+1) << 8);
    }

    return i;
}

int TSL2591_ALS(int gain, int time)
{
    int i;
    int p;

    p = TSL2591_Read(TSL2591_CTL);
    i = (gain < 4) | time;
    TSL2591_Write(i);

    return p;
}

void TSL2591_Reset(void)
{
    TSL2591_Read(TSL2591_CTL);
    TSL2591_Write(0x80);
}


/**
 * @brief Read/Write Functions
 * 
 */
int TSL2591_Read(int reg)
{
    int i;
    
    i = I2C_Poll(tsl, TSL2591_ADDRESS);
    if (i != 0)
  	    return -1;
    i = I2C_WriteByte(tsl, TSL2591_CMD | reg);
    I2C_Stop(tsl);
    i = I2C_Poll(tsl, TSL2591_ADDRESS | 1);
    i = I2C_ReadByte(tsl, 1);
    I2C_Stop(tsl);
    return i;
}

int TSL2591_Write(int value)
{
    int i;
    
    i = I2C_Poll(tsl, TSL2591_ADDRESS);
    if (i != 0)
    	return -1;
    i = I2C_WriteByte(tsl, value);
    I2C_Stop(tsl);
    return i;
}