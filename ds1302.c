/**
 * @brief Clock Calender module
 * @author Michael Burmeister
 * @date January 14, 2017
 * @version 1.2
 * 
*/

#include <propeller2.h>
#include <string.h>
#include "ds1302.h"
#include "ds1302reg.h"
#include <sys/time.h>

int _MM[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
char AMPM[][3] = {"AM", "PM"};
char _Msg[32];
int _AMPM;
int _SCLK;
int _MOSI;
int _CS;
int _MISO;

int DS1302_read(int);
void DS1302_write(int, int);
void spi_out(int, int, int, int);
int spi_in(int, int, int);


void DS1302_open(int mosi, int cs, int sclk, int miso)
{
  _SCLK = sclk;
  _CS = cs;
  _MOSI = mosi;
  _MISO = miso;
  _pinl(_CS);
  memset(_Msg, 0, sizeof(_Msg));
  _AMPM = -1;
}

int DS1302_getSeconds()
{
  int i, j;
  
  i = DS1302_read(DS1302SECONDS);
  i = i & 0x7f;
  j = i >> 4;
  i = (i & 0x0f) + j * 10;
  return i;
}

int DS1302_getMinutes()
{
  int i, j;
  
  i = DS1302_read(DS1302MINUTES);
  i = i & 0x7f;
  j = i >> 4;
  i = (i & 0x0f) + j * 10;
  return i;
}

int DS1302_getHours()
{
  int i, j;
  
  i = DS1302_read(DS1302HOURS);
  if ((i & 0x80) != 0)
  {
    if ((i & 0x20) != 0)
      _AMPM = 1;
    else
      _AMPM = 0;
    j = i & 0x0f;
    i = i >> 4;
    i = j + i * 10;
  }
  else
  {
    j = i & 0x0f;
    i = i & 0x3f;
    i = i >> 4;
    i = j + i * 10;
  }

  return i;
}

char* DS1302_getAMPM()
{
  return AMPM[_AMPM];
}
  
int DS1302_getDay()
{
  int i, j;
  
  i = DS1302_read(DS1302DAY);
  j = i >> 4;
  i = (i & 0x0f) + j * 10;
  return i;
}

int DS1302_getMonth()
{
  int i, j;
  
  i = DS1302_read(DS1302MONTH);
  j = i >> 4;
  i = (i & 0xf) + j * 10;
  return i;
}

int DS1302_getWeekDay()
{
  int i, j;
  
  i = DS1302_read(DS1302WEEKDAY);
  return i;
}

int DS1302_getYear()
{
  int i, j;
  
  i = DS1302_read(DS1302YEAR);
  j = i >> 4;
  i = (i & 0xf) + j * 10;
  return i;
}

void DS1302_setDate(int year, int month, int day)
{
  DS1302_setYear(year);
  DS1302_setMonth(month);
  DS1302_setDay(day);
}

void DS1302_setYear(int yr)
{
  int i, j;
  
  i = yr / 10;
  j = yr % 10;
  i = (i << 4) + j;
  DS1302_write(DS1302YEAR, i);
}

void DS1302_setMonth(int mn)
{
  int i, j;
  
  i = mn / 10;
  j = mn % 10;
  i = (i << 4) + j;
  DS1302_write(DS1302MONTH, i);
}

void DS1302_setDay(int dy)
{
  int i, j;
  
  i = dy / 10;
  j = dy % 10;
  i = (i << 4) + j;
  DS1302_write(DS1302DAY, i);
}

void DS1302_setWeekDay(int weekday)
{
  int i;
  
  i = weekday & 0x0f;
  DS1302_write(DS1302WEEKDAY, i);
}

void DS1302_setTime(int hours, int minutes, int seconds)
{
  DS1302_setHour(hours);
  DS1302_setMinute(minutes);
  DS1302_setSecond(seconds);
}

void DS1302_setHour(int hr)
{
  int i, j;
  
  i = hr / 10;
  j = hr % 10;
  i = (i << 4) + j;
  DS1302_write(DS1302HOURS, i);
}

void DS1302_set12Hour(int hr, char AmPm)
{
  int i, j;
  
  i = hr / 10;
  j = hr % 10;
  i = (i << 4) + j;
  if (AmPm == 'P')
    i = i | 0x20;
  i = i | 0x80;
  DS1302_write(DS1302HOURS, i);
}

void DS1302_setMinute(int mn)
{
  int i, j;
  
  i = mn / 10;
  j = mn % 10;
  i = (i << 4) + j;
  DS1302_write(DS1302MINUTES, i);
}

void DS1302_setSecond(int s)
{
  int i, j;
  
  i = s / 10;
  j = s % 10;
  i = (i << 4) + j;
  DS1302_write(DS1302SECONDS, i);
}
    
int DS1302_getWriteProtect()
{
  int i;
  
  i = DS1302_read(DS1302WP);
  if (i == 0x80)
    return -1;
  else
    return 0;
}

void DS1302_setWriteProtect()
{
  DS1302_write(DS1302WP, 0x80);
}

void DS1302_clearWriteProtect()
{
  DS1302_write(DS1302WP, 0x00);
}

void DS1302_setDateTime(void)
{
  struct timeval tv;
  
  int v = ((30 + DS1302_getYear()) * 36525 + 25)/100;
  v = v + _MM[DS1302_getMonth()-1] + DS1302_getDay() - 1;
  if ((DS1302_getYear() % 4) == 0)
    if (DS1302_getMonth() > 2)
      v = v + 1;
  v = v * 24 + DS1302_getHours();
  v = v * 60 + DS1302_getMinutes();
  v = v * 60 + DS1302_getSeconds();
  tv.tv_usec = 0;
  tv.tv_sec = v;
  settimeofday(&tv, 0);
}
    
/*
 * @brief read data
 * 
 */
int DS1302_read(int control)
{
  int i;
  
  _pinh(_CS);
  //shift_out(_MISO, _SCLK, LSBFIRST, 8, control);
  spi_out(_MISO, _SCLK, 8, control);
  //i = shift_in(_MOSI, _SCLK, LSBPRE, 8);
  i = spi_in(_MOSI, _SCLK, 8);
  _pinl(_CS);
  return i;
}

/*
 * @brief write data
 * 
 */
void DS1302_write(int control, int data)
{
  
  _pinh(_CS);
  //shift_out(_MISO, _SCLK, LSBFIRST, 8, control-1);
  spi_out(_MISO, _SCLK, 8, control-1);
  //shift_out(_MISO, _SCLK, LSBFIRST, 8, data);
  spi_out(_MISO, _SCLK, 8, data);
  _pinl(_CS);
}

void DS1302_setMessage(char *msg)
{
  int i;

  _pinh(_CS);
  //shift_out(_MISO, _SCLK, LSBFIRST, 8, DS1302BURSTMM-1);
  spi_out(_MISO, _SCLK, 8, DS1302BURSTMM-1);
  for (i=0;i<31;i++)
  {
    //shift_out(_MISO, _SCLK, LSBFIRST, 8, msg[i]);
    spi_out(_MISO, _SCLK, 8, msg[i]);
  }    
  _pinl(_CS);
}

char *DS1302_getMessage()
{
  int i;
  
  _pinh(_CS);
  //shift_out(_MISO, _SCLK, LSBFIRST, 8, DS1302BURSTMM);
  spi_out(_MISO, _SCLK, 8, DS1302BURSTMM);
  for (i=0;i<31;i++)
  {
    //_Msg[i] = shift_in(_MOSI, _SCLK, LSBPRE, 8);
    _Msg[i] = spi_in(_MOSI, _SCLK, 8);
  }    
  _pinl(_CS);
  return _Msg;
}

void spi_out(int mosi, int sclk, int len, int data)
{
  _pinh(mosi);
  _pinl(sclk);
  _waitus(1);
  for (int i=0;i<len;i++)
  {
    if ((data & 0x01) == 1)
      _pinh(mosi);
    else
      _pinl(mosi);
    _waitus(1);
    _pinnot(sclk);
    _waitus(1);
    _pinnot(sclk);
    _waitus(1);
    data = data >> 1;
  }
}

int spi_in(int miso, int sclk, int len)
{
  int data;

  data = 0;
  _dirl(miso);
  _pinl(sclk);
  _waitus(1);
  for (int i=0;i<len;i++)
  {
    data = data | (_pinr(miso) << i);
    _pinnot(sclk);
    _waitus(1);
    _pinnot(sclk);
    _waitus(1);
  }
  return data;
}

/*
void DS1302_burstRead()
{
  int i;
  
  high(_CS);
  shift_out(_MISO, _SCLK, LSBFIRST, 8, DS1302BURSTRD);
  i = shift_in(_MOSI, _SCLK, LSBPRE, 32);
  putHex(i);
  putChar('\n');
  low(_CS);
} 
*/
