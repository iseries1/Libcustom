/**
 * @brief SPI Driver
 * @author Michael Burmeister
 * @date March 22, 2022
 * @version 1.0
 * 
*/

typedef struct
{
    char mosi;
    char miso;
    char clk;
    char order;
    char wait;
} spi_t;

/**
 * @brief Construct a new Spi_init object
 * 
 * @param mosi master out slave in
 * @param miso master in slave out
 * @param clk clock pin
 * @param order 
 * @param wait timing value
 * @return spi_t pointer
 */
spi_t *Spi_Init(char mosi, char miso, char clk, char order, char wait);

/**
 * @brief Serial Input
 * @param spi Spi device
 * @return data
 */
int Spi_In(spi_t *spi, char bits);

/**
 * @brief Serial Output
 * @param spi Spi device
 * @param bits number of bits to send
 * @param data data value to send
 */
void Spi_Out(spi_t *spi, char bits, unsigned int data);
