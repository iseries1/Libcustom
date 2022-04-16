/**
 * @brief BH1750 Light Sensor Driver
 * @author Michael Burmeister
 * @date April 15, 2022
 * @version 1.0
 * 
*/

#include "BH1750.h"
#include "i2c.h"
#include <stdio.h>
#include <propeller.h>

int BH1750_Write(int);
int BH1750_Read(void);


#define BH1750_ADDRESS 0x46
#define BH1750_POWERDOWN 0x00
#define BH1750_POWERUP 0x01
#define BH1750_RESET 0x07
#define BH1750_HRESMODE 0x10
#define BH1750_HRESMODE2 0x11
#define BH1750_LRESMODE 0x13
#define BH1750_SHRESMODE 0x20
#define BH1750_SHRESMODE2 0x21
#define BH1750_SLRESMODE 0x23
#define BH1750_MEASTIMEH 0x40
#define BH1750_MEASTIMEL 0x60

i2c_t *bht;

int BH1750_Init(int scl, int sda)
{
    int i;

    bht = I2C_Init(scl, sda, I2C_STD);
    i = BH1750_Read();

    return i;
}

void BH1750_Power(int state)
{
    if (state == 0)
        BH1750_Write(BH1750_POWERDOWN);
    if (state == 1)
        BH1750_Write(BH1750_POWERUP);
}

void BH1750_Reset(void)
{
    BH1750_Write(BH1750_RESET);
}

void BH1750_Continues(int mode)
{
    if (mode == 0)
        BH1750_Write(BH1750_HRESMODE);
    if (mode == 1)
        BH1750_Write(BH1750_HRESMODE2);
    if (mode == 2)
        BH1750_Write(BH1750_LRESMODE);
}

void BH1750_Measure(int mode)
{
    if (mode == 0)
        BH1750_Write(BH1750_SHRESMODE);
    if (mode == 1)
        BH1750_Write(BH1750_SHRESMODE2);
    if (mode == 2)
        BH1750_Write(BH1750_SLRESMODE);
}

int BH1750_GetMeasurement(void)
{
    int i;

    i = BH1750_Read();

    return i;
}

void BH1750_SetMeasureTime(int t)
{
    int i;

    t = t & 0xff;
    i = t >> 5;
    BH1750_Write(BH1750_MEASTIMEH + i);
    i = t & 0x1f;
    BH1750_Write(BH1750_MEASTIMEL + i);
}


/**
 * @brief Read/Write data
 * 
 */

int BH1750_Read()
{
    int i;

    i = I2C_Poll(bht, BH1750_ADDRESS | 1);
    if (i != 0)
  	    return -1;
    i = I2C_ReadByte(bht, 0);
    i = (i << 8) | I2C_ReadByte(bht, 1);
    I2C_Stop(bht);
    return i;
}

int BH1750_Write(int cmd)
{
    int i;
    
    i = I2C_Poll(bht, BH1750_ADDRESS);
    if (i != 0)
    	return -1;
    i = I2C_WriteByte(bht, cmd);
    I2C_Stop(bht);
    return i;
}