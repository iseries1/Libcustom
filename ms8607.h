/**
 * @brief MS8607 Temperature Humidity and Presure sensor
 * @author Michael Burmeister
 * @date April 11, 2022
 * @version 1.0
 * 
*/

#include "ms8607_reg.h"

/**
 * @brief Init MS8607 sensor
 * @param clk clock pin
 * @param sda data pin
 * @return connection
 */
int ms8607_Init(int scl, int sda);

/**
 * @brief Reset ms8607 sensor
 */
int ms8607_Reset(void);

/**
 * @brief Read Calibration Data
 */
int ms8607_Calibration(void);

/**
 * @brief Get Pressure reading X 100
 * @param size 0-256, 1-512, 2-1024, 3-2048, 4-4096, 5-8192
 * @return pressure 24 bits
 */
int ms8607_GetPressure(int size);

/**
 * @brief Get Temperature reading X 100
 * @param size 0-256, 1-512, 2-1024, 3-2048, 4-4096, 5-8192
 * @return temperature 24 bits
 */
int ms8607_GetTemperature(int size);

/**
 * @brief Set User Registers humidity
 * @param value user register value
 */
int ms8607_SetUserRegister(int value);

/**
 * @brief Get Relative Humidity X 100
 * @return humidity
 */
int ms8607_GetHumidity(void);
