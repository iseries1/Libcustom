/**
 * @brief ST7789 Display Driver
 * @author Michael Burmeister
 * @date March 23, 2022
 * @version 1.0
 * 
*/


// Some ready-made 16-bit ('565') color settings:
#define ST7789_BLACK 0x0000
#define ST7789_WHITE 0xFFFF
#define ST7789_RED 0xF800
#define ST7789_GREEN 0x07E0
#define ST7789_BLUE 0x001F
#define ST7789_CYAN 0x07FF
#define ST7789_MAGENTA 0xF81F
#define ST7789_YELLOW 0xFFE0
#define ST7789_ORANGE 0xFC00

/**
 * @brief set connection parameters
 * @param Clk
 * @param MISO
 * @param MOSI
 * @param CS
 * @param DC
 * @param RST
 * @param LITE
 * @return open
 */
void ST7789_open(char Clk, char MISO, char MOSI, char CS, char DC, char RST, char LITE);

/**
 * @brief set rotation
 * @param rotation
 */
void ST7789_rotation(char rotation);

/**
 * @brief invert display
 * @param yes
 */
void ST7789_invert(char yes);

/**
 * @brief set window location
 * @param x
 * @param y
 * @param width
 * @param height
 */
void ST7789_window(short x, short y, short width, short height);

/**
 * @brief push color
 * @brief color
 */
void ST7789_pushColor(short color);

/**
 * @brief write pixel color
 * @param color
 */
void ST7789_writePixel(short color);

/**
 * @brief write color
 * @param color
 * @param len
 */
void ST7789_writeColor(short color, int len);

/**
 * @brief plot point color
 * @param x
 * @param y
 * @param color
 */
void ST7789_plot(short x, short y, short color);

/**
 * @brief fill rectangle
 * @param x
 * @param y
 * @param width
 * @param height
 * @param color
 */
void ST7789_fillRectangle(short x, short y, short width, short height, short color);

/**
 * @brief draw pixel color
 * @param x
 * @param y
 * @param color
 */
void ST7789_drawPixel(short x, short y, short color);

/**
 * @brief clear screen
 * @param color
 */
void ST7789_cls(short color);

/**
 * @brief set display on
 * @param mode 0/1 off/on
 */
void ST7789_displayOn(char mode);

/**
 * @brief put display to sleep
 * @param sleep 0/1 off/on
 */
void ST7789_sleepOn(char sleep);

/**
 * @brief set inverse on/off
 * @param inverse 0/1 on/off
 */
void ST7789_inverse(char inverse);

/**
 * @brief set text color
 * @param fgcolor
 * @param bgcolor
 */
void ST7789_textColor(short fgcolor, short bgcolor);

/**
 * @brief write small string 8x8 (5x7)
 * @param x
 * @param y
 * @param text
 */
void ST7789_writeSStr(short x, short y, char *text);

/**
 * @brief write small character 8x8 (5x7)
 * @param x
 * @param y
 * @param character
 */
void ST7789_writeSChar(short x, short y, char character);

/**
 * @brief write character 16x32
 * @param x offset
 * @param y offset
 * @param c character
 */
void ST7789_writeChar(short x, short y, char c);

/**
 * @brief write string 16x32
 * @param x offset
 * @param y offset
 * @param s pointer to string
 */
void ST7789_writeStr(short x, short y, char* s);

/**
 * @brief write extra large character 32x64
 * @param x offset
 * @param y offset
 * @param c character
 */
void ST7789_writeXChar(short x, short y, char c);

/**
 * @brief write extra large string 32x64
 * @param x offset
 * @param y offset
 * @param s pointer to string
 */
void ST7789_writeXStr(short x, short y, char *s);

/**
 * @brief draw line 
 * @param x start point
 * @param y start point
 * @param x1 end point
 * @param y1 end point
 * @param color line color
 */
void ST7789_drawLine(short x, short y, short x1, short y1, short color);

/**
 * @brief draw a box
 * @param x start point
 * @param y start point
 * @param x1 end point
 * @param y1 end point
 * @param color line color
 */
void ST7789_drawBox(short x, short y, short x1, short y1, short color);

/**
 * @brief draw a circle
 * 
 * @param x center point of circle
 * @param y center point of circle
 * @param ra radius of circle
 * @param color line color
 */
void ST7789_drawCircle(short x, short y, short ra, short color);

/***
 * @brief build 888 rgb color value
 * @param red 8 bits
 * @param green 8 bits
 * @param blue 8 bits
 * @return color value
 */
unsigned short ST7789_color(unsigned char red, unsigned char green, unsigned char blue);
