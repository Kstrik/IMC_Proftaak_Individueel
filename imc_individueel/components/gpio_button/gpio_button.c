#include "include/gpio_button.h"

void gpio_button_check_pressed(gpio_button_t** button);
void gpio_button_check_task(void* pvParameter);

// Starts task to check the input form the button
void gpio_button_start_listener(gpio_button_t** button)
{
    // Check if task checking the input of the button is not running
    if(!(*button)->is_listening)
    {
        (*button)->is_pressed = false;      // Set boolean value indicating if the button is pressed
        (*button)->is_listening = true;     // Set boolean value indicating the task is running
        xTaskCreate(&gpio_button_check_task, "buttonCheckTask", 2048, button, 5, NULL);     // Create task to check the input form the button
    }
}

// Stops task to check the input form the button
void gpio_button_stop_stop_listener(gpio_button_t** button)
{
    // Check if task checking the input of the button is running
    if((*button)->is_listening)
        (*button)->is_listening = false;    // Set boolean value indicating the task is not running
}

// Checks the input on the GPIO pin connnected to the button and if it is pressed
void gpio_button_check_pressed(gpio_button_t** button)
{
    gpio_pad_select_gpio((gpio_num_t)(*button)->gpio_pin);                  // Select GPIO pin to be read
    gpio_set_direction((gpio_num_t)(*button)->gpio_pin, GPIO_MODE_INPUT);   // Set GPIO pin direction

    int input = gpio_get_level((gpio_num_t)(*button)->gpio_pin);            // Read value of GPIO pin
    // Check id input is high indicating the buttun is pressed and if it was already pressed
    if(input > 0 && !(*button)->is_pressed)
    {
        (*button)->is_pressed = true;
        if((*button)->onButtonPressed != NULL)
            (*button)->onButtonPressed();
    }
    else if(input == 0 && (*button)->is_pressed)
        (*button)->is_pressed = false;
}

// Function for the task checkking the input of the button
void gpio_button_check_task(void* pvParameter)
{
    // Cast void pointer that was passed to the task as parameter to the corresponding button
    gpio_button_t** button = (gpio_button_t**)pvParameter;

    // Check if the task still needs to be checking if the button is pressed
    while((*button)->is_listening)
    {
        gpio_button_check_pressed(button);
        vTaskDelay(100 / portTICK_RATE_MS);
    }
    vTaskDelete(NULL);  // Delete the task, it is not needed anymore
}