/**
 * @brief MS8607 Temperature Humidity and Presure sensor
 * @author Michael Burmeister
 * @date April 11, 2022
 * @version 1.0
 * 
*/

//#define DEBUG

#include "ms8607.h"
#include "i2c.h"
#include <stdio.h>
#include <propeller.h>

int ms8607_readtp(int, int);
int ms8607_writetp(int);
int ms8607_readh(int, int);
int ms8607_writeh(int);
int ms8607_writeuh(int);


i2c_t *ms8607t;
int MSPC1;
int MSPC2;
int MSPC3;
int MSPC4;
int MSPC5;
int MSPC6;

unsigned int MSHC0;
unsigned int MSHC1;
unsigned int MSHC2;
unsigned int MSHC3;
unsigned int MSHC4;
unsigned int MSHC5;
unsigned int MSHC6;

long long MSPOFF;
long long MSPSEN;


int ms8607_Init(int scl, int sda)
{
    ms8607t = I2C_Init(scl, sda, I2C_STD);
    return 0;
}

int ms8607_Reset(void)
{
    int i;

    i = ms8607_writetp(PSENSOR_RESET_COMMAND);
    if (i < 0)
        return i;
    i = ms8607_writeh(HSENSOR_RESET_COMMAND);

    return i;
}

int ms8607_Calibration(void)
{
    int i;

    i = ms8607_readtp(PROM_ADDRESS, 2);
    MSPC1 = ms8607_readtp(PROM_ADDRESS + 2, 2);
    MSPC2 = ms8607_readtp(PROM_ADDRESS + 4, 2);
    MSPC3 = ms8607_readtp(PROM_ADDRESS + 6, 2);
    MSPC4 = ms8607_readtp(PROM_ADDRESS + 8, 2);
    MSPC5 = ms8607_readtp(PROM_ADDRESS + 10, 2);
    MSPC6 = ms8607_readtp(PROM_ADDRESS + 12, 2);

    i = i << 16;
//    MSHC0 = ms8607_readh(PROM_ADDRESS, 2);
//    MSHC1 = ms8607_readh(PROM_ADDRESS + 2, 2);
//    MSHC2 = ms8607_readh(PROM_ADDRESS + 4, 2);
//    MSHC3 = ms8607_readh(PROM_ADDRESS + 6, 2);
//    MSHC4 = ms8607_readh(PROM_ADDRESS + 8, 2);
//    MSHC5 = ms8607_readh(PROM_ADDRESS + 10, 2);
//    MSHC6 = ms8607_readh(PROM_ADDRESS + 12, 2);

//    i = i | (MSHC6 & 0x0f);

    MSHC6 = MSHC6 >> 4;

#ifdef DEBUG
    __builtin_printf("C1: %d, C2: %d, C3: %d, C4: %d, C5: %d, C6: %d\n", MSPC1, MSPC2, MSPC3, MSPC4, MSPC5, MSPC6);
#endif

    return i;
}

int ms8607_GetPressure(int size)
{
    int i;

    i = CONVERT_D1_OSR_256 + (size << 1);
    ms8607_writetp(i);
    wait(500);
    i = ms8607_readtp(1, 3);

#ifdef DEBUG
    __builtin_printf("D1: %d\n", i);
#endif

    i = (((i * MSPSEN) >> 21) - MSPOFF) >> 15;

    return i;
}

int ms8607_GetTemperature(int size)
{
    int D2;
    long long t;

    D2 = CONVERT_D2_OSR_256 + (size << 1);
    ms8607_writetp(D2);
    wait(50);
    D2 = ms8607_readtp(0, 3);

#ifdef DEBUG
    __builtin_printf("D2: %d\n", D2);
#endif

    t = D2 - (MSPC5 << 8);
    D2 = 2000 + ((t * MSPC6) >> 23);

    MSPOFF = MSPC2;
    MSPOFF = (MSPOFF << 17) + ((MSPC4 * t) >> 6);
    MSPSEN = MSPC1;
    MSPSEN = (MSPSEN << 16) + ((MSPC3 * t) >> 7);

#ifdef DEBUG
    __builtin_printf("Off: %lld, Sen: %lld\n", MSPOFF, MSPSEN);
#endif

    return D2;
}

int ms8607_SetUserRegister(int value)
{
    int i;

    i = ms8607_writeuh( value);

    return i;
}

int ms8607_GetHumidity(void)
{
    int i;

    i = ms8607_readh(HSENSOR_READ_HUMIDITY_WO_HOLD_COMMAND, 3);

#ifdef DEBUG
    __builtin_printf("Hum: %x\n", i);
#endif

    i = i >> 8;
    if ((i & 0x02) == 0)
        printf("Humidity Error\n");
    i = i & 0xfffc;

    i = -600 + ((12500 * i) >> 16);

    return i;
}


/** i2c Read Write
 */

int ms8607_readtp(int cmd, int count)
{
    int i;
    
    i = I2C_Poll(ms8607t, MS8607_PT_ADDRESS);
    if (i != 0)
  	    return -1;
    i = I2C_WriteByte(ms8607t, cmd);
    I2C_Stop(ms8607t);
    i = I2C_Poll(ms8607t, MS8607_PT_ADDRESS | 1);
    for (int j=1;j<count;j++)
        i = (i << 8) | I2C_ReadByte(ms8607t, 0);

    i = (i << 8) | I2C_ReadByte(ms8607t, 1);
    I2C_Stop(ms8607t);
    return i;
}

int ms8607_writetp(int cmd)
{
    int i;
    
    i = I2C_Poll(ms8607t, MS8607_PT_ADDRESS);
    if (i != 0)
    	return -1;
    i = I2C_WriteByte(ms8607t, cmd);
    if (i != 0)
    	return -1;
    I2C_Stop(ms8607t);
    return i;
}

int ms8607_readh(int cmd, int count)
{
    int i;
    int t;

    i = I2C_Poll(ms8607t, MS8607_HUM_ADDRESS);
    if (i != 0)
   	    return -1;
    i = I2C_WriteByte(ms8607t, cmd);

    for (t=0;t<50;t++)
    {
        i = I2C_Poll(ms8607t, MS8607_HUM_ADDRESS | 1);
        if (i == 0)
            break;
        wait(5);
    }
    if (i != 0)
        __builtin_printf("Error\n");

    for (int j=1;j<count;j++)
        i = (i << 8) | I2C_ReadByte(ms8607t, 0);
    i = (i << 8) | I2C_ReadByte(ms8607t, 1);
    I2C_Stop(ms8607t);
    return i;
}

int ms8607_writeh(int cmd)
{
    int i;
    
    i = I2C_Poll(ms8607t, MS8607_HUM_ADDRESS);
    if (i != 0)
    	return -1;
    i = I2C_WriteByte(ms8607t, cmd);
    if (i != 0)
    	return -1;
    I2C_Stop(ms8607t);
    return i;
}

int ms8607_writeuh(int value)
{
    int i;
    int v;

    i = I2C_Poll(ms8607t, MS8607_HUM_ADDRESS);
    if (i != 0)
    	return -1;
    i = I2C_WriteByte(ms8607t, HSENSOR_READ_USER_REG_COMMAND);
    if (i != 0)
    	return -1;
    i = I2C_Poll(ms8607t, MS8607_HUM_ADDRESS | 1);
    v = I2C_ReadByte(ms8607t, 1);
    i = v & 0x3A;
    i = i | value;
    I2C_Poll(ms8607t, MS8607_HUM_ADDRESS);
    i = I2C_WriteByte(ms8607t, HSENSOR_WRITE_USER_REG_COMMAND);
    i = I2C_WriteByte(ms8607t, value);
    I2C_Stop(ms8607t);

    return v;
}
