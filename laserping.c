/**
 * @brief Laser Ping Driver to determine distance
 * @author Michael Burmeister
 * @date March 30, 2019
 * @version 1.0
 * 
*/

#include <stdio.h>
#include <propeller2.h>
#include <stdlib.h>
#include "laserping.h"


void laserping_run(void *par);
void laserping_runp(void *par);
void PulseOut(int, int);
int PulseIn(int, int);


char _Pin;
int _Average[16];
char _Buffer[25];
int _RS;
unsigned Stack[51];
FILE *_s;

void laserping_start(char mode, char pin)
{
  _Pin = pin;

  if (mode == 'S')
    cogstart(laserping_run, 0, Stack, 50);
  else
    cogstart(laserping_runp, 0, Stack, 50);
}
  
void laserping_run(void *par)
{
  char c;
  int p;
  int i;
  int t;
  int bitperiod = (_clkfreq / 9600);
  int bit_mode;

  dirh(_Pin);
  pinl(_Pin);
  wait(250);

  // calculate smartpin mode for 8 bits per character
  bit_mode = 7 + (bitperiod << 16);
  pinstart(_Pin, P_OE | P_ASYNC_TX, bit_mode, 0);

  wypin(_Pin, 'I');
  wait(10);
  wypin(_Pin, 'I');
  wait(10);

  pinl(_Pin);
  pinstart(_Pin, P_ASYNC_RX, bit_mode, 0);

  p = 0;
  t = 0;
  _RS = 1;
  while (_RS)
  {
    do {
      i = pinr(_Pin);
    } while (i == 0);

    c = rdpin(_Pin) >> 24; // shift down to byte
    if (c == 13)
    {
      _Buffer[p] = 0;
      i = atoi(_Buffer);
      _Average[t++] = i;
      t = t & 0x0f;
      p = 0;
    }
    else
      _Buffer[p++] = c;

    if (p > 10)
      p = 0;
  }
  pinl(_Pin);
  //cogstop(cogid());
}

void laserping_runp(void *par)
{
  int i;
  int t;
  
  pinl(_Pin);
  
  t = 0;
  _RS = 1;
  while (_RS)
  {
    wait(70);
    pinl(_Pin);
    PulseOut(_Pin, 5);
    i = PulseIn(_Pin, 1);
    _Average[t++] = i * 1715 / 10000;
    t = t & 0x0f;
  }
}
  
int laserping_distance(void)
{
  int d = 0;
  
  for (int i=0;i<16;i++)
    d = d + _Average[i];
  
  return d / 16;
}

void laserping_stop(void)
{
  _RS = 0;
}

// Pulse pin for x
void PulseOut(int p, int t)
{
    int i;
    
    i = pinr(p);
    if (i == 0)
    	pinh(p);
    else
    	pinl(p);
    waitus(t);
    if (i == 0)
    	pinl(p);
    else
    	pinh(p);
}

// Get Pulse Length in microseconds
int PulseIn(int p, int s)
{
    int i;
    
    dirl(p);
    i = getus();
    while ((pinr(p) != s) && ((getus() - i) < 1000));
    i = getus();
    while ((pinr(p) == s) && ((getus() - i) < 100000));
    i = getus() - i;
    return i;
}
