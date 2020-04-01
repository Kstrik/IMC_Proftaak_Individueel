#ifndef MATRIX_DISPLAY_H
#define MATRIX_DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "i2c_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

// Type for representing the position data and state of a LED for the matrix display
typedef struct
{
    uint8_t x;                  // X coordinate on matrix display
    uint8_t y;                  // Y coordinate on matrix display
    bool is_on;                 // Boolean value indicating if the LED at [x, y] is on or off
} matrix_display_value_pair_t;

// Type for representing the value of a row on the matrix display and if it has changed after the last update
typedef struct
{
    uint8_t data;               // Value of matrix display row
    bool has_changed;           // Boolean value for indicating if the row data has changed
} row_pair_t;

// Type for representing the matrix display with its I2C address and buffer of row data
typedef struct
{
    uint8_t i2c_address;        // I2C address of the matrix display
    row_pair_t* buffer;         // Buffer for row values of the display and booleans for indicating change in data on a row
    unsigned int buffer_length; // Length of buffer indicating the number of elements
    bool is_initialized;        // Boolean value for indicating if the matrix display is initialized
} matrix_display_t;

// Initializes the matrix display given to the function
void matrix_display_init(matrix_display_t* display);
// Deinitializes the matrix display given to the function
void matrix_display_deinit(matrix_display_t* display);

// Sets the value (on/off : 1/0) of a pixel on the matrix display at a certain x and y position
void matrix_display_set_pixel(matrix_display_t* display, uint8_t x, uint8_t y, bool is_on);
// Sets the values (on/off : 1/0) of multiple pixels on the matrix display
void matrix_display_set_pixels(matrix_display_t* display, matrix_display_value_pair_t* pixel_values, unsigned int length);
// Updates the matrix display with the data in the buffer
void matrix_display_update(matrix_display_t* display);
// Sets the values of all the pixels of the matrix display given to the function to (off : 0) essentially clearing the matrix display
void matrix_display_clear(matrix_display_t* display);

#ifdef __cplusplus
}
#endif

#endif  // MATRIX_DISPLAY_H