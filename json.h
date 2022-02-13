/**
 * @file json.h
 * @brief Convert Json data to values
 * @author Michael Burmeister
 * @date December 29, 2018
 * @version 1.5
 * 
*/

/**
 * @brief init json converter/writer
 * @param json data
 */
void json_init(char *json);

/**
 * @brief find element
 * @param element full name
 * @return string value
 */
char *json_find(char *element);

/**
 * @brief put string element and value
 * @param item Item name
 * @param value Item value
 */
void json_putStr(char *item, char *value);

/**
 * @brief put integer value
 * @param item Item name
 * @param value decimal value
 */
void json_putDec(char *item, char *value);

/**
 * @brief put array of objects (start of array)
 * Provide name of array to start the array and
 * then write each item in the array using that
 * function and then end the array here with no
 * item name
 * @param item object array or NULL for end
 */
void json_putArray(char* item);

/**
 * @brief put object values
 * @param item object name NULL for end of object
 */
void json_putObject(char* item);

/**
 * @brief put more items
 */
void json_putMore(void);

