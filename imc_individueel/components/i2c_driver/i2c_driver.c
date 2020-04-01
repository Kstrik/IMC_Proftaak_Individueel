/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#include "include/i2c_driver.h"

static SemaphoreHandle_t i2cSemaphore;     // Mutex for allowing only one task to read or write data across i2c bus
static bool is_initialized = false;         // Boolean indicating if the I2CDriver is initialized

// Initializes the i2c configuration
i2c_result_t i2c_driver_init(i2c_mode_t mode, uint8_t sda_pin, uint8_t scl_pin, 
            gpio_pullup_t sda_pullup_en, gpio_pullup_t scl_pullup_en, 
            unsigned int clk_speed)
{
    // Checks if i2c_driver is already initialized, and if not initialize it
    if(!is_initialized)
    {
        i2cSemaphore = xSemaphoreCreateMutex();	// Create mutex for reading and writing to and from i2c devices

        i2c_config_t config;
        config.mode = mode;
        config.sda_io_num = (gpio_num_t)sda_pin;
        config.scl_io_num = (gpio_num_t)scl_pin;
        config.sda_pullup_en = sda_pullup_en;
        config.scl_pullup_en = scl_pullup_en;
        config.master.clk_speed = clk_speed;

        i2c_set_timeout(I2C_NUM_0, 20000);							// Set i2c timeout

        esp_err_t ret = i2c_param_config(I2C_NUM_0, &config);		// Set i2c configuration
        if (ret != ESP_OK)
        {
            ESP_LOGE("I2CDriver", "PARAM CONFIG FAILED");
            return I2C_DRIVER_ERR_CONFIG;
        }
        ESP_LOGV("I2CDriver", "PARAM CONFIG DONE");

        ret = i2c_driver_install(I2C_NUM_0, config.mode, I2C_MASTER_TX_BUF_DISABLE, I2C_MASTER_RX_BUF_DISABLE, INTR_FLAGS);
        if (ret != ESP_OK) {
            ESP_LOGE("I2CDriver", "I2C DRIVER INSTALL FAILED");
            return I2C_DRIVER_ERR_INSTALL;
        }
        ESP_LOGV("I2CDriver", "I2C DRIVER INSTALLED");

        is_initialized = true;
    }
    return I2C_DRIVER_OK;
}

// Deinitializes the i2c configuration
i2c_result_t i2c_driver_deinit()
{
	// Checks if i2c_driver is already initialized, and if it is deinitialize it
    if(is_initialized)
	{
		vSemaphoreDelete(i2cSemaphore);	// Destroy mutex for reading and write to and from i2c devices
		is_initialized = false;
	}
	return I2C_DRIVER_OK;
}

// Write 8 bits to register [reg] at address [addr]
i2c_result_t i2c_driver_write_register8(uint8_t addr, uint8_t reg, uint8_t data)
{
    // Check if I2CDriver is already initialized, and if not write data
	if(is_initialized)
	{
		xSemaphoreTake(i2cSemaphore, portMAX_DELAY);	// Enter critical section and take the semaphore to block other theads from entering
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
		i2c_master_stop(cmd);
		esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);
		xSemaphoreGive(i2cSemaphore);					// Exit critical section and give the semaphore to unblock other theads from entering
		if (ret != ESP_OK) {
			ESP_LOGE("I2CDriver", "ERROR: unable to write to register %d", ret);
			return I2C_DRIVER_ERR_FAIL;
		}

		return I2C_DRIVER_OK;
	}
	return I2C_DRIVER_ERR_NOT_INITIALIZED;
}

// Write 16 bits to register [reg] at address [addr] 
i2c_result_t i2c_driver_write_register16(uint8_t addr, uint8_t reg, uint16_t data)
{
	// Check if I2CDriver is already initialized, and if not write data
	if(is_initialized)
	{
		xSemaphoreTake(i2cSemaphore, portMAX_DELAY);	// Enter critical section and take the semaphore to block other theads from entering
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, data >> 8, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, data & 0xFF, ACK_CHECK_EN);
		i2c_master_stop(cmd);
		esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);
		xSemaphoreGive(i2cSemaphore);					// Exit critical section and give the semaphore to unblock other theads from entering
		if (ret != ESP_OK) {
			ESP_LOGE("I2CDriver", "ERROR: unable to write to register %d", ret);
			return I2C_DRIVER_ERR_FAIL;
		}

		return I2C_DRIVER_OK;
	}
	return I2C_DRIVER_ERR_NOT_INITIALIZED;
}

// Write 24 bits to register [reg] at address [addr] 
i2c_result_t i2c_driver_write_register24(uint8_t addr, uint8_t reg, uint32_t data)
{
	// Check if I2CDriver is already initialized, and if not write data
	if(is_initialized)
	{
		xSemaphoreTake(i2cSemaphore, portMAX_DELAY);	// Enter critical section and take the semaphore to block other theads from entering
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, data >> 16, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, data >> 8, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, data & 0xFF, ACK_CHECK_EN);
		i2c_master_stop(cmd);
		esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);
		xSemaphoreGive(i2cSemaphore);					// Exit critical section and give the semaphore to unblock other theads from entering
		if (ret != ESP_OK)
		{
			ESP_LOGE("I2CDriver", "ERROR: unable to write to register %d", ret);
			return I2C_DRIVER_ERR_FAIL;
		}

		return I2C_DRIVER_OK;
	}
	return I2C_DRIVER_ERR_NOT_INITIALIZED;
}

// Read 8 bits from register [reg] at address [addr]
i2c_result_t i2c_driver_read_register8(uint8_t addr, uint8_t reg, uint8_t* data)
{
	// Check if I2CDriver is already initialized, and if not read data
	if(is_initialized)
	{
		xSemaphoreTake(i2cSemaphore, portMAX_DELAY);	// Enter critical section and take the semaphore to block other theads from entering
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
		i2c_master_stop(cmd);
		esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);
		if (ret != ESP_OK) {
			xSemaphoreGive(i2cSemaphore);				// Exit critical section and give the semaphore to unblock other theads from entering
			ESP_LOGE("I2CDriver", "ERROR: unable to write address %02x to read reg %02x %d", addr, reg, ret);
			return I2C_DRIVER_ERR_FAIL;
		}

		vTaskDelay(30 / portTICK_RATE_MS);
		
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | READ_BIT, ACK_CHECK_EN);
		i2c_master_read_byte(cmd, data, (i2c_ack_type_t)ACK_VAL);
		ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);
		xSemaphoreGive(i2cSemaphore);					// Exit critical section and give the semaphore to unblock other theads from entering
		if (ret != ESP_OK)
		{
			ESP_LOGE("I2CDriver", "ERROR: unable to write address %02x to read reg %02x %d", addr, reg, ret);
			return I2C_DRIVER_ERR_FAIL;
		}

		return I2C_DRIVER_OK;
	}
	return I2C_DRIVER_ERR_NOT_INITIALIZED;
}

// Read 16 bits from register [reg] at address [addr]
i2c_result_t i2c_driver_read_register16(uint8_t addr, uint8_t reg, uint16_t* data)
{
	// Check if I2CDriver is already initialized, and if not read data
	if(is_initialized)
	{
		xSemaphoreTake(i2cSemaphore, portMAX_DELAY);	// Enter critical section and take the semaphore to block other theads from entering
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | WRITE_BIT, ACK_CHECK_EN);
		i2c_master_write_byte(cmd, reg, ACK_CHECK_EN);
		i2c_master_stop(cmd);
		esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);
		if (ret != ESP_OK)
		{
			xSemaphoreGive(i2cSemaphore);				// Exit critical section and give the semaphore to unblock other theads from entering
			ESP_LOGE("I2CDriver", "ERROR: unable to write address %02x to read reg %02x %d", addr, reg, ret);
			return I2C_DRIVER_ERR_FAIL;
		}
		
		vTaskDelay(30 / portTICK_RATE_MS);
		
		uint8_t lsb = 0x00;
		uint8_t msb = 0x00;

		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (addr << 1) | READ_BIT, ACK_CHECK_EN);
		i2c_master_read_byte(cmd, &lsb, (i2c_ack_type_t)ACK_VAL);
		i2c_master_read_byte(cmd, &msb, (i2c_ack_type_t)ACK_VAL);
		ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);
		xSemaphoreGive(i2cSemaphore);					// Exit critical section and give the semaphore to unblock other theads from entering
		if (ret != ESP_OK)
		{
			ESP_LOGE("I2CDriver", "ERROR: unable to write address %02x to read reg %02x %d", addr, reg, ret);
			return I2C_DRIVER_ERR_FAIL;
		}

		*data = ((uint16_t)msb << 8 | lsb);

		return I2C_DRIVER_OK;
	}
	return I2C_DRIVER_ERR_NOT_INITIALIZED;
}