/**
 * @brief Driver for I2C devices
 * @author Michael Burmeister
 * @date January 21, 2021
 * @version 1.0
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <propeller2.h>
#include "i2c.h"


i2c_t *I2C_Init(int scl, int sda, int spd)
{
    int i;
    i2c_t *x;

	i = 10;
    x = malloc(sizeof(int));
    if (spd == I2C_STD)
    	i = 10;
    if (spd == I2C_FAST)
    	i = 2;
    if (spd == I2C_FASTP)
    	i = 1;
    if (spd == I2C_HIGH)
    	i = 0;
    i = scl | (sda << 8) | (i << 16);
    *x = (i2c_t)i;
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
    int i;
    
    i = (int)*x;
    s = i >> 16;
    d = (i >> 8) & 0xff;
    c = i &0xff;
    
    _pinh(d);
//    _waitus(s);
    _pinh(c);
    _waitus(s);
    _pinl(d);
    _waitus(s);
    _pinl(c);
    _waitus(s);
}

void I2C_Stop(i2c_t *x)
{
    int s, c, d;
    int i;
    
    i = (int)*x;
    s = i >> 16;
    d = (i >> 8) & 0xff;
    c = i &0xff;

    _pinl(c);
    _pinl(d);
    _waitus(s);
    _pinh(c);
    _waitus(s);
    _pinh(d);
    _waitus(s);
}

int I2C_WriteByte(i2c_t *x, int b)
{
    int s, c, d;
    int i;
    
    i = (int)*x;
    s = i >> 16;
    d = (i >> 8) & 0xff;
    c = i &0xff;

    for (i=0;i<8;i++)
    {
        if (b & 0x80)
        	_dirl(d); // float pin
        else
        	_dirh(d); // enable out
        _waitus(s);
        _pinh(c);
        _waitus(s);
        b = b << 1;
        _pinl(c);
    }
	_dirl(d); // float answer
    _waitus(s);
	_pinh(c);
	_waitus(s);
	d = _pinr(d); //ACK=low
	_pinl(c);
	_waitus(s);
	return d;
}


unsigned char I2C_ReadByte(i2c_t *x, int ack)
{
    int s, c, d;
    int b;
    int i;
    
    i = (int)*x;
    s = i >> 16;
    d = (i >> 8) & 0xff;
    c = i &0xff;

	_dirl(d);
    _waitus(s);

	b = 0;
	for (i=0;i<8;i++)
	{
	    b = b << 1;
	    _pinh(c);
	    _waitus(s);
	    b = b | _pinr(d);
	    _pinl(c);
	    _waitus(s);
	}
	
	if (ack)
		_dirl(d);
	else
		_dirh(d);
    _waitus(s);
	_pinh(c);
	_waitus(s);
	_pinl(c);
	_waitus(s);
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

int I2C_In(i2c_t *x, int address, int reg, int size, unsigned char *data, int count)
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

int I2C_Out(i2c_t *x, int address, int reg, int size, unsigned char *data, int count)
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
