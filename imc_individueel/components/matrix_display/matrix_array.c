#include "include/matrix_array.h"

// Initializes the matrix array given to the function
void matrix_array_init(matrix_array_t** array, display_orientation_t orientation)
{
    // Check if matrix array is inititialied and if not initialize it
    if(!(*array)->is_initialized)
    {
        (*array)->orientation = orientation;   // Set orentation
        (*array)->matrix_displays = NULL;      // Set pointer of matix display array to a null pointer (empty)
        (*array)->matrix_display_count = 0;    // Set matrix display count to 0
        (*array)->is_initialized = true;       // Set initialization state to intialized
    }
}

// Deinitializes the matrix array given to the function
void matrix_array_deinit(matrix_array_t** array)
{
    // Check if matrix array is inititialied
    if((*array)->is_initialized)
    { 
        if((*array)->matrix_displays != NULL)
        {
            // Loop through all the matrix displays and deinitialize them
            for(int i = 0; i < (*array)->matrix_display_count; i++)
                matrix_display_deinit(&(*array)->matrix_displays[i]);

            free((*array)->matrix_displays);   // Free the memory of the pointer for the array of matrix displays
        }

        (*array)->matrix_display_count = 0;    // Set matrix display count back to 0
        (*array)->is_initialized = false;      // Set initialization state to unintialized
    }
}

// Checks if one of the matrix displays that is already part of the array has the same i2c address
bool matrix_array_display_exists(matrix_array_t** array, uint8_t i2c_address)
{
    // Check if matrix array is inititialied and there are matrix display's present in the matrix array
    if((*array)->is_initialized && (*array)->matrix_displays != NULL)
    {
        // Loop through all the matrix displays in the array and check there i2c addresses
        for(int i = 0; i < (*array)->matrix_display_count; i++)
        {
            if((*array)->matrix_displays[i].i2c_address == i2c_address)
                return true;
        }
    }
    return false;
}

// Adds a matrix display to the array
void matrix_array_add_matrix_display(matrix_array_t** array, uint8_t i2c_address)
{
    /*
        Check if matrix array is inititialied and if matrix display with corresponding i2c address already exists in the array, 
        if not adds a new matrix display to the array
    */
    if((*array)->is_initialized && !matrix_array_display_exists(array, i2c_address))
    {
        matrix_display_t display = {
            .i2c_address = i2c_address      // Set i2c address of the matrix display
        };
        matrix_display_init(&display);      // Initialize the matrix display
        (*array)->matrix_display_count++;   // Increment the matrix display count

        // Allocate enough memory to the matrix display's array
        if((*array)->matrix_displays == NULL)
            (*array)->matrix_displays = (matrix_display_t*)malloc(sizeof(matrix_display_t*));
        else
            (*array)->matrix_displays = (matrix_display_t*)realloc((*array)->matrix_displays, sizeof(matrix_display_t) * (*array)->matrix_display_count);
        (*array)->matrix_displays[(*array)->matrix_display_count - 1] = display;    // Adds matrix display to the matris display array
    }
}

// Sets the value (on/off : 1/0) of a pixel on the corresponding matrix display on the array at a certain x and y position
void matrix_array_set_pixel(matrix_array_t** array, int x, int y, bool is_on)
{
    // Check if matrix array is inititialied
    if((*array)->is_initialized)
    {
        // Checks if the matrix array has a horizontal orientation to know how to index the matrix display array correctly
        if((*array)->orientation == HORIZONTAL)
        {
            // Checks if x and y values are not outside possible matrix array coördinates (size is [display count] x 8)
            if(x < ((*array)->matrix_display_count * 8) && y < 8)
                matrix_display_set_pixel(&(*array)->matrix_displays[x / 8], x % 8, y, is_on);
        }
        else
        {
            // Checks if x and y values are not outside possible matrix array coördinates (size is 8 x [display count])
            if(x < 8 && y < ((*array)->matrix_display_count * 8))
                matrix_display_set_pixel(&(*array)->matrix_displays[y / 8], x, y % 8, is_on);
        }
    }
}

// Sets the values (on/off : 1/0) of multiple pixels on the corresponding matrix display on the array
void matrix_array_set_pixels(matrix_array_t** array, matrix_display_value_pair_t* pixel_values, unsigned int length)
{
    // Check if matrix array is inititialied
    if((*array)->is_initialized)
    {
        // Loop trough all pixels values pairs given and set corresponding LED values in the matrix display buffers
        for(int i = 0; i < length; i++)
            matrix_array_set_pixel(array, pixel_values[i].x, pixel_values[i].y, pixel_values[i].is_on);
    }
}

// Updates the matrix displays with the data in the buffers
void matrix_array_update(matrix_array_t** array)
{
    // Check if matrix array is inititialied
    if((*array)->is_initialized)
    {
        // Loop through all the matrix displays in the array and update them
        for(int i = 0; i < (*array)->matrix_display_count; i++)
            matrix_display_update(&(*array)->matrix_displays[i]);
    }
}

// Sets the values of all the pixels of the matrix displays to (off : 0) essentially clearing the matrix displays
void matrix_array_clear(matrix_array_t** array)
{
    // Check if matrix array is inititialied
    if((*array)->is_initialized)
    {
        // Loop through all the matrix displays in the array and clear them
        for(int i = 0; i < (*array)->matrix_display_count; i++)
            matrix_display_clear(&(*array)->matrix_displays[i]);
    }
}