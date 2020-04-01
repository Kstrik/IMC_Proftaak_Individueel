/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#ifndef GPIO_BUTTON_H
#define GPIO_BUTTON_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Type representing a button connected to a GPIO pin
typedef struct
{
    uint8_t gpio_pin;                   // GPIO pin that the button is connected to
    bool is_pressed;                    // Boolean value indicating if the button is pressed
    void(*onButtonPressed)(void);       // Function pointer to be called when the button is pressed
    bool is_listening;                  // Boolean value indicating if the task checking the input form the button is running
} gpio_button_t;

// Starts task to check the input form the button
void gpio_button_start_listener(gpio_button_t** button);
// Stops task to check the input form the button
void gpio_button_stop_stop_listener(gpio_button_t** button);

#ifdef __cplusplus
}
#endif

#endif  // GPIO_BUTTON_H