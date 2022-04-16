/**
 * @file bh1750.h
 * @brief BH1750 Light Sensor Driver
 * @author Michael Burmeister
 * @date April 15, 2022
 * @version 1.0
 * 
*/


/**
 * @brief Initialize HB1750 device
 * @param scl clock pin
 * @param sda data pin
 */
int BH1750_Init(int scl, int sda);

/**
 * @brief Power Up/Down device
 * @param power 0 - down, 1 - up
 */
void BH1750_Power(int state);

/**
 * @brief Reset Device
 */
void BH1750_Reset(void);

/**
 * @brief Set Continues Measurement Mode
 * @param mode 0 - High Res, 1 - High Res two, 2 - Low Res
 */
void BH1750_Continues(int mode);

/**
 * @brief Do Measurement mode
 * @param mode 0 - High Res, 1 - High Res two, 2 - Low Res
 */
void BH1750_Measure(int mode);

/**
 * @brief Get Measurement
 * @return value
 */
int BH1750_GetMeasurement(void);

/**
 * @brief Set Measurement Time
 * @param time default(69) - 120ms
 */
void BH1750_SetMeasureTime(int t);
