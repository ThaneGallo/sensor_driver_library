#ifndef AHT21_H
#define AHT21_H

#include <stdint.h>  // for uint8_t, uint16_t
#include <stdlib.h>
#include <driver/i2c_master.h>
#include "esp_log.h"

//for delays and whatnot
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

// I2C address for AHT21
#define AHT21_I2C_ADDRESS 0x38
static const char *TAG = "AHT21_Sensor";

//misc i2c setup info
#define I2C_MASTER_SCL_IO 22        /*!< GPIO number for I2C master clock */
#define I2C_MASTER_SDA_IO 21        /*!< GPIO number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0    /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000   /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

//Errors for sensor measurement 
#define RELATIVE_HUMIDITY_ACCURACY_ERROR .02 //2%
#define TEMPERATURE_ACCURACY_ERROR .3 //.3c

// Function Prototypes

/**
 * @brief Initializes the AHT21 sensor.
 * @return 0 if initialization is successful, -1 otherwise.
 */
int8_t AHT21_Init(void);

/**
 * @brief Triggers a measurement and reads temperature and humidity values.
 * @param temperature Pointer to store temperature in Celsius.
 * @param humidity Pointer to store relative humidity in %.
 * @return 0 if measurement is successful, -1 otherwise.
 */
int8_t AHT21_Read(float* temperature, float* humidity);


/**
 * @brief Checks if CRC is valid
 * @param data Pointer to data to be checked
 * @param polynomial value to be checked 
 * @param crc initial CRC value
 * @param length length of data to be checked
 * @return 0 if reset command was acknowledged, -1 otherwise.
 */
int8_t CRC_check(uint8_t* data, uint8_t polynomial, uint8_t crc, uint8_t length);

void i2c_setup();

#ifdef __cplusplus
}
#endif

#endif // AHT21_H
