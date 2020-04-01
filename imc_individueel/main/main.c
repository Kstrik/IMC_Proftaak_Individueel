#include "esp_log.h"
#include "nvs_flash.h"

#include "i2c_driver.h"
#include "flappy_bird.h"

void init_nvs_flash();

// Entry point of application
void app_main(void)
{
    init_nvs_flash();                                                                               // Initialize nvs_flash
    i2c_driver_init(I2C_MODE_MASTER, 23, 22, GPIO_PULLUP_ENABLE, GPIO_PULLUP_ENABLE, 9600);         // Initialize i2c_driver

    flappy_bird_init();                         // Initialize the flappy bird game
    flappy_bird_start();                        // Start the flappy bird game

    // This code should be run to stop and release the resources of the flappy bird game
    // flappy_bird_stop();                         // Stop the flappy bird game
    // flappy_bird_deinit();                       // Uninitialize the flappy bird game
    // i2c_driver_deinit();                        // Uninitialize i2c_driver
}

// Initializes nvs_flash
void init_nvs_flash()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    esp_log_level_set("*", ESP_LOG_ERROR);
    esp_log_level_set("*", ESP_LOG_INFO);
}