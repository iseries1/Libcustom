
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/driver.h>
#include <propeller.h>
#include "serial.h"

char s_prefix[] = "SS";

// Define the driver list
_Driver _SSDriver =
{
    s_prefix,
    NULL,
    serial_close,
    serial_read,
    serial_write,
    NULL,
    NULL,
    serial_getcf,
    serial_putcf
};

FILE *serial_open(int rxpin, int txpin, int baudrate)
{
    int i;
    int bitperiod = (_clkfreq / baudrate);
    int bit_mode;
    FILE *fp;

    for (i=3;i<FOPEN_MAX;i++)
      if (__files[i]._drv == NULL)
        break;
    if (i == FOPEN_MAX)
    {
        _seterror(EMFILE);
        return NULL;
    }

    fp = &__files[i];
//    b = malloc(sizeof(struct _default_buffer));
//    b->cnt = 0;
//    b->ptr = b->buf;


    fp->_bsiz = 0;
    fp->_drv = &_SSDriver;
    fp->_flag = O_RDWR;

    i = (rxpin & 0xff) | ((txpin & 0xff) << 8); // Save the pin config here
    fp->drvarg[0] = i << 16;

    // calculate smartpin mode for 8 bits per character
    bit_mode = 7 + (bitperiod << 16);
    
    // set up the transmit pin
    if (txpin >= 0)
      pinstart(txpin, P_OE | P_ASYNC_TX, bit_mode, 0);

    // set up the receive pin
    if (rxpin >= 0)
      pinstart(rxpin, P_ASYNC_RX, bit_mode, 0);

    return fp;
}

int serial_close(FILE *device)
{
    int i;
    unsigned tx_pin;
    unsigned rx_pin;

    i = device->drvarg[0] >> 16;
    rx_pin = i & 0xff;
    tx_pin = i >> 8;

    if (tx_pin < 64)
      dirl(tx_pin);
    if (rx_pin < 64)
      dirl(rx_pin);

    device->_drv = 0;

    return 0;
}

int serial_rxReady(FILE *device)
{
    int z = 0;
    unsigned rx_pin;

    rx_pin = device->drvarg[0] >> 16;
    rx_pin = rx_pin & 0xff;
    z = pinr(rx_pin);
    
    return z;
}

int serial_rxCheck(FILE *device)
{
    int z = 0;
    unsigned rxbyte;
    unsigned rx_pin;

    rx_pin = device->drvarg[0] >> 16;
    rx_pin = rx_pin & 0xff;
    z = pinr(rx_pin);
    if (z == 0)
      return -1;
    
    rxbyte = rdpin(rx_pin) >> 24; // shift down to byte
    rxbyte = rxbyte & 0xff;
    return rxbyte;
}

int serial_rxChar(FILE *device)
{
    int z = 0;
    unsigned rxbyte;
    unsigned rx_pin;

    rx_pin = device->drvarg[0] >> 16;
    rx_pin = rx_pin & 0xff;

    while ((z = pinr(rx_pin)) == 0)
	   waitus(50);
    
	rxbyte = rdpin(rx_pin) >> 24; // shift down to byte
	rxbyte = rxbyte & 0xff;

	return rxbyte;
}

int serial_txChar(FILE *device, unsigned char txbyte)
{
    int z = 0;
    unsigned tx_pin;

    tx_pin = device->drvarg[0] >> 16;
    tx_pin = tx_pin >> 8;

    wypin(tx_pin, txbyte);

    for (int i=0;i<100;i++)
    {
      z = pinr(tx_pin);
      if (z != 0)
        return 0;
      waitus(50);
    }

    return -1;
}

int serial_putcf(int c, FILE *device)
{
  return serial_txChar(device, c);
}

int serial_getcf(FILE *device)
{
  return serial_rxChar(device);
}

int serial_read(FILE *device, unsigned char *buff, int count)
{
    unsigned char *b = (unsigned char*)buff;

    b[0] = serial_rxChar(device);
    return 1;
}

int serial_write(FILE *device, unsigned char *buff, int count)
{
    int z = 0;
    unsigned tx_pin;
    const unsigned char *b = (const unsigned char*)buff;

    tx_pin = device->drvarg[0];
    tx_pin = tx_pin >> 8;

    for (int i=0;i<count;i++)
    {
      wypin(tx_pin, b[i]);
      while ((z = pinr(tx_pin)) == 0)
        waitus(50);
    }

    return count;
}
