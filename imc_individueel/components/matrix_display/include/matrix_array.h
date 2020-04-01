#ifndef MATRIX_ARRAY_H
#define MATRIX_ARRAY_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "matrix_display.h"

#ifdef __cplusplus
extern "C" {
#endif

// Enumerator for different orientations of the matrix array
typedef enum
{
    HORIZONTAL,
    VERTICAL
} display_orientation_t;

// Type for representing the matrix array
typedef struct
{
    display_orientation_t orientation;      // Orientation of the matrix array
    matrix_display_t* matrix_displays;      // Pointer pointing to the fisrt matrix display in the matrix array
    unsigned int matrix_display_count;      // Ammount of matrix display's that are part of the matrix array
    bool is_initialized;                    // Boolean value for indicating if the matrix array is initialized
} matrix_array_t;

// Initializes the matrix array given to the function
void matrix_array_init(matrix_array_t** array, display_orientation_t orientation);
// Deinitializes the matrix array given to the function
void matrix_array_deinit(matrix_array_t** array);

// Adds a matrix display to the array
void matrix_array_add_matrix_display(matrix_array_t** array, uint8_t i2c_address);
// Sets the value (on/off : 1/0) of a pixel on the corresponding matrix display on the array at a certain x and y position
void matrix_array_set_pixel(matrix_array_t** array, int x, int y, bool is_on);
// Sets the values (on/off : 1/0) of multiple pixels on the corresponding matrix display on the array
void matrix_array_set_pixels(matrix_array_t** array, matrix_display_value_pair_t* pixel_values, unsigned int length);
// Updates the matrix displays with the data in the buffers
void matrix_array_update(matrix_array_t** array);
// Sets the values of all the pixels of the matrix displays to (off : 0) essentially clearing the matrix displays
void matrix_array_clear(matrix_array_t** array);

#ifdef __cplusplus
}
#endif

#endif  // MATRIX_ARRAY_H