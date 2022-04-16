/**
 * @brief SPI Driver
 * @author Michael Burmeister
 * @date March 22, 2022
 * @version 1.0
 * 
*/

#include "spi.h"
#include <stdio.h>
#include <stdlib.h>
#include <propeller.h>


spi_t *Spi_Init(char mosi, char miso, char clk, char order, char wait)
{
    spi_t *x = malloc(sizeof(spi_t));
    x->mosi = mosi;
    x->miso = miso;
    x->clk = clk;
    x->order = order;
    if (wait == 0)
        wait = 2;
    x->wait = wait;

    pinl(clk);
    dirl(miso);
    dirh(mosi);

    return x;
}

int Spi_In(spi_t *spi, char bits)
{
    int i;
    unsigned data = 0;
    int wt = spi->wait;
    int bt = bits;
    int clk = spi->clk;
    int pin = spi->miso;

    i = 32 - bits;

    if (spi->order == 0)
    {
        asm volatile("drvl %[clk]\n"
            "l1: testp %[pin], wc\n"
            "rcl %[data], #1\n"
            "drvh %[clk]\n"
            "drvl %[clk]\n"
            "waitx %[w]\n"
            "djnz %[b], #l1\n"
            :[data]"+r"(data), [b]"+r"(bt)
            :[clk]"r"(clk), [pin]"r"(pin), [w]"r"(wt));
    }
    if (spi->order == 1)
    {
        asm volatile("drvl %[clk]\n"
            "l2: testp %[pin] wc\n"
            "rcr %[data], #1\n"
            "drvh %[clk]\n"
            "drvl %[clk]\n"
            "waitx %[w]\n"
            "djnz %[b], #l2\n"
            "shr %[data], %[i]\n"
            :[data]"+r"(data), [b]"+r"(bt)
            :[clk]"r"(clk), [pin]"r"(pin), [w]"r"(wt), [i]"r"(i));
    }

    return data;
}

void Spi_Out(spi_t *spi, char bits, unsigned int data)
{
    int i;
    int wt = spi->wait;
    int bt = bits;
    int clk = spi->clk;
    int pin = spi->mosi;

    i = 32 - bits;

    if (spi->order == 0)
    {
        asm volatile("drvl %[clk]\n"
            "shl %[data], %[i]\n"
            "l3: rcl %[data], #1 wc\n"
            "if_c drvh %[pin]\n"
            "if_nc drvl %[pin]\n"
            "drvh %[clk]\n"
            "waitx %[w]\n"
            "drvl %[clk]\n"
            "waitx %[w]\n"
            "djnz %[b], #l3\n"
            :[data]"+r"(data), [i]"+r"(i), [b]"+r"(bt)
            :[clk]"r"(clk), [pin]"r"(pin), [w]"r"(wt));
    }
    if (spi->order == 1)
    {
        asm volatile("drvl %[clk]\n"
            "l4: rcr %[data], #1 wc\n"
            "if_c drvh %[pin]\n"
            "if_nc drvl %[pin]\n"
            "drvh %[clk]\n"
            "waitx %[w]\n"
            "drvl %[clk]\n"
            "waitx %[w]\n"
            "djnz %[b], #l4\n"
            :[data]"+r"(data), [i]"+r"(i), [b]"+r"(bt)
            :[clk]"r"(clk), [pin]"r"(pin), [w]"r"(wt));
    }
}