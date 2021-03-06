/**
 * @brief Flash Driver
 * @author Michael Burmeister
 * @date January 26, 2021
 * @version 1.0
 * 
*/

#include <stdlib.h>
#include <stdio.h>
#include <propeller.h>
#include "Flash.h"

void spi_outM(int, int, int, int);
int spi_inM(int, int, int);


flash_t* Flash_Init(int MOSI, int MISO, int CLK, int CS)
{
    int i;
    
    flash_t *x;
    
    x = malloc(sizeof(flash_t));
    i = MOSI | (MISO << 8) | (CLK << 16) | (CS << 24);
    *x = (flash_t)i;
    dirh(CS);
    dirh(MOSI);
    dirl(MISO);
    dirh(CLK);
    pinh(CS);
    
    pinl(CS);
    spi_outM(MOSI, CLK, 0x90, 8);
    spi_outM(MOSI, CLK, 0x0, 24);
    i = spi_inM(MISO, CLK, 16);
    pinh(CS);
    i = i >> 8;
    if (i != 0xef)
       return NULL;
    return x;
}

void Flash_Reset(flash_t *x)
{
    int m, s, k, c;
    int i;
    
    i = (int)*x;
    m = i & 0xff;
    s = (i >> 8) & 0xff;
    k = (i >> 16) & 0xff;
    c = (i >> 24);
    
    pinl(c);
    spi_outM(m, k, 0x66, 8);
    pinh(c);
    pinl(c);
    spi_outM(m, k, 0x99, 8);
    pinh(c);
    waitus(50);
}

int Flash_Busy(flash_t *x)
{
    int m, s, k, c;
    int i;
    
    i = (int)*x;
    m = i & 0xff;
    s = (i >> 8) & 0xff;
    k = (i >> 16) & 0xff;
    c = (i >> 24);

    pinl(c);
    spi_outM(m, k, 0x05, 8);
    i = spi_inM(s, k, 8);
    pinh(c);
    return i;
}

int Flash_Read(flash_t *x, int address, unsigned char *buffer, int len)
{
   int m, s, k, c;
    int i;
    
    i = (int)*x;
    m = i & 0xff;
    s = (i >> 8) & 0xff;
    k = (i >> 16) & 0xff;
    c = (i >> 24);
    
    pinl(c);
    spi_outM(m, k, 0x03, 8);
    spi_outM(m, k, address, 24);
    for (i=0;i<len;i++)
    	buffer[i] = spi_inM(s, k, 8);
    pinh(c);
    return len;
}

int Flash_Write(flash_t *x, int address, unsigned char *buffer, int len)
{
   int m, s, k, c;
    int i;
    
    i = (int)*x;
    m = i & 0xff;
    s = (i >> 8) & 0xff;
    k = (i >> 16) & 0xff;
    c = (i >> 24);

     pinl(c);
     spi_outM(m, k, 0x06, 8);
     pinh(c);
     waitus(1);
     pinl(c);
     spi_outM(m, k, 0x02, 8);
     spi_outM(m, k, address, 24);
     for (i=0;i<len;i++)
        spi_outM(m, k, buffer[i], 8);
     pinh(c);
     for (i=0;i<500;i++)
     {
        if (Flash_Busy(x) == 0)
            return len;
        waitus(1);
     }
     return -1;
}

void Flash_Erase(flash_t *x, int address, int len)
{
   int m, s, k, c;
    int i;
    
    i = (int)*x;
    m = i & 0xff;
    s = (i >> 8) & 0xff;
    k = (i >> 16) & 0xff;
    c = (i >> 24);
    
    pinl(c);
    spi_outM(m, k, 0x06, 8); //write enable set
    pinh(c);
    waitus(1);
    pinl(c);
    if (len == 4096)
    	spi_outM(m, k, 0x20, 8);
    if (len == 32768)
    	spi_outM(m, k, 0x52, 8);
    if (len == 65536)
    	spi_outM(m, k, 0xd8, 8);
    spi_outM(m, k, address, 24);
	pinh(c);
    waitus(1);

	while (Flash_Busy(x))
		waitus(1);
}


/**
 * @brief SPI Functions
 */
void spi_outM(int pinDat, int pinClk, int data, int len)
{
    data = data << (32 - len);
    
    pinh(pinDat);
    pinl(pinClk);
    for (int i=0;i<len;i++)
    {
        if ((data & 0x80000000) == 0)
        	pinl(pinDat);
        else
        	pinh(pinDat);
        pinh(pinClk);
        pinl(pinClk);
        data = data << 1;
	}
}

int spi_inM(int pinDat, int pinClk, int len)
{
    int data;
    
    data = 0;
    dirl(pinDat);
    pinl(pinClk);
    for (int i=len-1;i>=0;i--)
    {
        pinh(pinClk);
        data = data | (pinr(pinDat) << i);
        pinl(pinClk);
	}
    return data;
}
