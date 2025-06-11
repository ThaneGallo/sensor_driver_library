#include "AHT21.h"

i2c_master_dev_handle_t dev_handle = NULL;
i2c_master_bus_handle_t bus_handle = NULL;

void i2c_setup()
{

    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = 0,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = 1,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x38,
        .scl_speed_hz = 100000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
    ESP_LOGI(TAG, "I2C device initialized, handle = %p", dev_handle);
}

/**
 * @brief Initializes the AHT21 sensor.
 * @return 0 if initialization is successful, -1 otherwise.
 */
int8_t AHT21_Init(void)
{
    // settle for 100-->500ms scl is high
    vTaskDelay(pdMS_TO_TICKS(500));

    uint8_t cmd = 0x71;

    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, &cmd, 1, -1));
    ESP_LOGI(TAG, "0x71 complete");

    ESP_ERROR_CHECK(i2c_master_receive(dev_handle, &cmd, 1, -1));
    ESP_LOGI(TAG, "Received byte: 0x%02X", cmd);

    if ((cmd & 0x18) != 0x18)
    {
        ESP_LOGI(TAG, "State byte is invalid");
        return -1;
    }

    return 0;
}

/**
 * @brief Triggers a measurement and reads temperature and humidity values.
 * @param temperature Pointer to store temperature in Celsius.
 * @param humidity Pointer to store relative humidity in %.
 * @return 0 if measurement is successful, -1 otherwise.
 */
int8_t AHT21_Read(float *temperature, float *humidity)
{

    int8_t i;

    // for storage of read bytes
    uint8_t read_data[7]; // 2 of each

    
    uint8_t status_check;
    uint8_t cmd[3] = {0xAC, 0x33, 0x00};

    do
    {

        vTaskDelay(pdMS_TO_TICKS(10));

        ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, cmd, 3, -1));

        vTaskDelay(pdMS_TO_TICKS(80));

        ESP_ERROR_CHECK(i2c_master_receive(dev_handle, read_data, 7, -1));

        if ((read_data[0] & 0x80) == 0)
        {
            status_check = 0xFF;
        }
    } while (status_check != 0xFF);


    i = CRC_check(read_data, 0x31, 0xFF, 7);
    if (i == -1)
    {
        ESP_LOGI(TAG, "CRC Check Failed");
    }


    int32_t S_th = 0;
    int32_t S_h = 0;

    // Combine humidity (20 bits): buffer[0] << 12 | buffer[1] << 4 | buffer[2] >> 4
    S_h = ((uint32_t)read_data[1] << 12) |
          ((uint32_t)read_data[2] << 4) |
          ((read_data[3] & 0xF0) >> 4);

    // Combine temperature (20 bits): (buffer[2] & 0x0F) << 16 | buffer[3] << 8 | buffer[4]
    S_th = ((uint32_t)(read_data[3] & 0x0F) << 16) |
           ((uint32_t)read_data[4] << 8) |
           read_data[5];

    float divisor = 0x100000;

    // assigns new values to humidity
    *humidity = (float)(((float)S_h / divisor) * 100.0f);

    *temperature = (float)(((float)S_th / divisor) * 200.0f) - 50.0f;

    return 0;
}

int8_t CRC_check(uint8_t *data, uint8_t polynomial, uint8_t crc, uint8_t length)
{

    for (uint8_t i = 0; i < length - 1; i++)
    {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ polynomial;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    // Compare calculated CRC with last byte in data (CRC)
    if (crc == data[length - 1])
    {
        return 0; // CRC matches, data valid
    }
    else
    {
        ESP_LOGI(TAG, "Calculated CRC %u", crc);
        ESP_LOGI(TAG, "Expected CRC %u", data[length - 1]);
        return -1; // CRC mismatch, data invalid
    }
}