/**
 * @file tsl2591.h
 * @brief TSL2591 Light Sensor Driver
 * @author Michael Burmeister
 * @date April 14, 2022
 * @version 1.0
 * 
*/

/**
 * @brief Initialize Light Sensor
 * @param scl clock pin
 * @param sda data pin
 * @return device id
 */
int TSL2591_Init(int scl, int sda);

/**
 * @brief Get/Set Enable
 * @param value set value
 * @return previous value
 */
int TSL2591_Enable(int value);

/**
 * @brief Get Status Value
 * @return status of device
 */
int TSL2591_Status(void);

/**
 * @brief Read Light Level
 * @param Channel Diode Channel to read 0/1
 * @return value ambient light value
 */
int TSL2591_ReadALS(int Channel);

/**
 * @brief Get/Set Sensor Gain and time
 * @param gain ALS gain 0 - 3
 * @param time Sample time 0 - 5
 * @return previous value
 */
int TSL2591_ALS(int gain, int time);

/**
 * @brief Reset Sensor
 */
void TSL2591_Reset(void);
