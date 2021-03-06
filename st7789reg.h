/**
 * @brief ST7789 Display Driver
 * @author Michael Burmeister
 * @date March 23, 2022
 * @version 1.0
 * 
*/

#define ST7789_TFTWIDTH 240
#define ST7789_TFTHEIGHT 320

#define ST7789_NOP 0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID 0x04
#define ST7789_RDDST 0x09

#define ST7789_SLPIN 0x10
#define ST7789_SLPOUT 0x11
#define ST7789_PTLON 0x12
#define ST7789_NORON 0x13

#define ST7789_INVOFF 0x20
#define ST7789_INVON 0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON 0x29
#define ST7789_CASET 0x2A
#define ST7789_RASET 0x2B
#define ST7789_RAMWR 0x2C
#define ST7789_RAMRD 0x2E

#define ST7789_PTLAR 0x30
#define ST7789_TEOFF 0x34
#define ST7789_TEON 0x35
#define ST7789_MADCTL 0x36
#define ST7789_COLMOD 0x3A

#define ST7789_MADCTL_MY 0x80
#define ST7789_MADCTL_MX 0x40
#define ST7789_MADCTL_MV 0x20
#define ST7789_MADCTL_ML 0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RAMCTRL 0xB0
#define ST7789_RGBCTRL 0xB1
#define ST7789_PORCTRL 0xB2
#define ST7789_FRCTRL1 0xB3

#define ST7789_GCTRL 0xB7
#define ST7789_DGMEN 0xBA
#define ST7789_VCOMS 0xBB

#define ST7789_LCMCTRL 0xC0
#define ST7789_IDSET 0xC1
#define ST7789_VDVVRHEN 0xC2

#define ST7789_VRHS 0xC3
#define ST7789_VDVSET 0xC4
#define ST7789_VCMOFSET 0xC5
#define ST7789_FRCTR2 0xC6
#define ST7789_CABCCTRL 0xC7
#define ST7789_REGSEL1 0xC8
#define ST7789_REGSEL2 0xCA
#define ST7789_PWMFRSEL 0xCC

#define ST7789_PWCTRL1 0xD0
#define ST7789_VAPVANEN 0xD2
#define ST7789_PVGAMCTRL 0xE0
#define ST7789_NVGAMCTRL 0xE1
#define ST7789_DGMLUTR 0xE2
#define ST7789_DGMLUTB 0xE3
#define ST7789_GATECTRL 0xE4
#define ST7789_PWCTRL2 0xE8
#define ST7789_EQCTRL 0xE9

#define ST7789_RDID1 0xDA
#define ST7789_RDID2 0xDB
#define ST7789_RDID3 0xDC
#define ST7789_RDID4 0xDD


