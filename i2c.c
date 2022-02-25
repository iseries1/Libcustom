/**
 * @brief Driver for I2C devices
 * @author Michael Burmeister
 * @date January 21, 2021
 * @version 1.0
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <propeller.h>
#include "i2c.h"


i2c_t *I2C_Init(int scl, int sda, int spd)
{
    unsigned int i;
    i2c_t *x;

	i = 10;
    x = malloc(sizeof(i2c_t));
    if (spd == I2C_STD)
    	i = 10 * _clkfreq / 1000000;
    if (spd == I2C_FAST)
    	i = 2 * _clkfreq / 1000000;
    if (spd == I2C_FASTP)
    	i = 1 * _clkfreq / 1000000;
    if (spd == I2C_HIGH)
    	i = 1;
    x->wait = i;
    x->clk = scl;
    x->dta = sda;

    I2C_Stop(x);
    return x;
}

int I2C_Poll(i2c_t *x, int address)
{
    int ack;
    
    I2C_Start(x);
    ack = I2C_WriteByte(x, address);
    return ack;
}

void I2C_Start(i2c_t *x)
{
    int s, c, d;

    s = x->wait;
    d = x->dta;
    c = x->clk;
    
    pinh(d);
//    _waitus(s);
    pinh(c);
    waitx(s);
    pinl(d);
    waitx(s);
    pinl(c);
    waitx(s);
}

void I2C_Stop(i2c_t *x)
{
    int s, c, d;
    
    s = x->wait;
    d = x->dta;
    c = x->clk;

    pinl(c);
    pinl(d);
    waitx(s);
    pinh(c);
    waitx(s);
    pinh(d);
    waitx(s);
}

int I2C_WriteByte(i2c_t *x, int b)
{
    int s, c, d, i;
    
    s = x->wait;
    d = x->dta;
    c = x->clk;
    i = 8;

    asm ("shl %[b], #24\n"
        "_isnd: rcl %[b], #1 wc\n"
        "if_c dirl %[d]\n"
        "if_nc dirh %[d]\n"
        "waitx %[s]\n"
        "drvh %[c]\n"
        "waitx %[s]\n"
        "drvl %[c]\n"
        "djnz %[i], #_isnd\n"
        "dirl %[d]\n"
        "waitx %[s]\n"
        "drvh %[c]\n"
        "waitx %[s]\n"
        "testp %[d] wc\n"
        "wrc %[d]\n"
        "drvl %[c]\n"
        "waitx %[s]\n"
        :[d]"+r"(d),[i]"+r"(i), [b]"+r"(b)
        :[c]"r"(c), [s]"r"(s));

	return d;
}


unsigned char I2C_ReadByte(i2c_t *x, int ack)
{
    int s, c, d;
    int b, i;
    
    s = x->wait;
    d = x->dta;
    c = x->clk;
    i = 8;
    b = 0;

    asm("dirl %[d]\n"
        "waitx %[s]\n"
        "mov %[b], #0\n"
        "_ird: shl %[b], #1\n"
        "drvh %[c]\n"
        "waitx %[s]\n"
        "testp %[d] wc\n"
        "if_c or %[b], #1\n"
        "drvl %[c]\n"
        "waitx %[s]\n"
        "djnz %[i], #_ird\n"
        "cmp %[ack], #1 wz\n"
        "if_z dirl %[d]\n"
        "if_nz dirh %[d]\n"
        "waitx %[s]\n"
        "drvh %[c]\n"
        "waitx %[s]\n"
        "drvl %[c]\n"
        "waitx %[s]\n"
        :[b]"+r"(b), [i]"+r"(i)
        :[d]"r"(d), [s]"r"(s), [c]"r"(c), [ack]"r"(ack));

	return b;
}

int I2C_WriteData(i2c_t *x, unsigned char *data, int count)
{
    int i;
	int r;
	
	for (i=0;i<count;i++)
	{
	    r = I2C_WriteByte(x, data[i]);
	    if (r)
	        return i;
	}
	return i;   
}

int I2C_ReadData(i2c_t *x, unsigned char *data, int count)
{
    int i;

    for (i=0;i<count-1;i++)
    {
        data[i] = I2C_ReadByte(x, 0);
    }
    data[i++] = I2C_ReadByte(x, 1);
    return i;
}

int I2C_In(i2c_t *x, unsigned address, unsigned reg, unsigned size, unsigned char *data, int count)
{
    int i;
    unsigned char buffer[4];

    address = address << 1;
    address = address & 0xfe;

    I2C_Start(x);
    if (I2C_WriteByte(x, address))
	    return 0;
    buffer[0] = reg >> 24;
    buffer[1] = (reg >> 16) & 0xff;
    buffer[2] = (reg >> 8) & 0xff;
    buffer[3] = reg & 0xff;
    i = 4 - size;
    i = I2C_WriteData(x, &buffer[i], size);
    address = address | 0x01;
    I2C_Start(x);
    if (I2C_WriteByte(x, address))
	    return 0;
    i = I2C_ReadData(x, data, count);
    I2C_Stop(x);
    return i;
}

int I2C_Out(i2c_t *x, unsigned  address, unsigned reg, unsigned size, unsigned char *data, int count)
{
    int i;
    unsigned char buffer[4];

    address = address << 1;
    address = address & 0xfe;
    I2C_Start(x);
    if (I2C_WriteByte(x, address))
	    return 0;
    buffer[0] = reg >> 24;
    buffer[1] = (reg >> 16) & 0xff;
    buffer[2] = (reg >> 8) & 0xff;
    buffer[3] = reg & 0xff;
    i = 4 - size;
    i = I2C_WriteData(x, &buffer[i], size);
    i = I2C_WriteData(x, data, count);
    I2C_Stop(x);
    return i;
}
