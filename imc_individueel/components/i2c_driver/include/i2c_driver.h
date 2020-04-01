/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "driver/i2c.h"
#include "esp_log.h"

#include <stdint.h>

#define WRITE_BIT I2C_MASTER_WRITE
#define READ_BIT I2C_MASTER_READ
#define ACK_CHECK_EN 0x1
#define ACK_CHECK_DIS 0x0
#define ACK_VAL 0x0
#define NACK_VAL 0x1

#ifdef __cplusplus
extern "C" {
#endif

// Enumerator for the different error types that can be thrown by the i2c driver
typedef enum
{
    I2C_DRIVER_OK = 0x00,
    I2C_DRIVER_ERR_CONFIG = 0x01,
    I2C_DRIVER_ERR_INSTALL = 0x02,
    I2C_DRIVER_ERR_FAIL = 0x03,
    I2C_DRIVER_ERR_NOT_INITIALIZED = 0x04
} i2c_result_t;

static const size_t I2C_MASTER_TX_BUF_DISABLE = 0;
static const size_t I2C_MASTER_RX_BUF_DISABLE = 0;
static const int INTR_FLAGS = 0;

// Initializes the i2c configuration
i2c_result_t i2c_driver_init(i2c_mode_t mode, uint8_t sda_pin, uint8_t scl_pin, 
            gpio_pullup_t sda_pullup_en, gpio_pullup_t scl_pullup_en, 
            unsigned int clk_speed);
// Deinitializes the i2c configuration
i2c_result_t i2c_driver_deinit();

// Write 8 bits to register [reg] at address [addr]
i2c_result_t i2c_driver_write_register8(uint8_t addr, uint8_t reg, uint8_t data);
// Write 16 bits to register [reg] at address [addr] 
i2c_result_t i2c_driver_write_register16(uint8_t addr, uint8_t reg, uint16_t data);
// Write 24 bits to register [reg] at address [addr] 
i2c_result_t i2c_driver_write_register24(uint8_t addr, uint8_t reg, uint32_t data);

// Read 8 bits from register [reg] at address [addr]
i2c_result_t i2c_driver_read_register8(uint8_t addr, uint8_t reg, uint8_t* data);
// Read 16 bits from register [reg] at address [addr]
i2c_result_t i2c_driver_read_register16(uint8_t addr, uint8_t reg, uint16_t* data);

#ifdef __cplusplus
}
#endif

#endif  // I2C_DRIVER_H