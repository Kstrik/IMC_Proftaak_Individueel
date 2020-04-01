/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#include "include/matrix_display.h"

// Initializes the matrix display given to the function
void matrix_display_init(matrix_display_t* display)
{
    // Check if matrix display is initialized and if not initialize it
    if(!display->is_initialized)
    {
        display->buffer = (row_pair_t*)malloc(sizeof(row_pair_t) * 8);  // Allocate enough memory to the buffer for 8 rows
        display->buffer_length = 8;                                     // Set buffer length

        i2c_driver_write_register8(display->i2c_address, 0x21, 0x00);   // System setup command
        i2c_driver_write_register8(display->i2c_address, 0x81, 0x00);   // Turn on display with no blinking
        i2c_driver_write_register8(display->i2c_address, 0xE7, 0xFF);   // Set the matrix to full brightness

        // Loop trough all rows of the matrix and turn off the corresponding LED's
        for(int y = 0; y < 8; y++)
        {
            i2c_driver_write_register8(display->i2c_address, y * 2, 0x00);
            display->buffer[y].data = 0x00;
            display->buffer[y].has_changed = false;
        }
        display->is_initialized = true;     // Set state of display to uninitialized
    }
}

// Deinitializes the matrix display given to the function
void matrix_display_deinit(matrix_display_t* display)
{
    // Check if matrix display is initialized and if it is uninitialize it
    if(display->is_initialized)
    {
        free(display->buffer);              // Free memory of the buffer
        display->is_initialized = false;    // Set state of display to uninitialized
    }
}

// Sets the value (on/off : 1/0) of a pixel on the matrix display at a certain x and y position
void matrix_display_set_pixel(matrix_display_t* display, uint8_t x, uint8_t y, bool is_on)
{
    // Check if matrix display is initialized
    if(display->is_initialized)
    {
        // LED matrix is 8x8 so x or y values above 7 are not allowed (values are indexed from 0 to 7)
        if(x > 7 || y > 7)
            return;
        
        /* 
            First LED column for some reason is the last bit (0b10000000 : 0x80), so the value written 
            must be shifted one less to the left if the x value is higher than 0 and must be (0b10000000 : 0x80) otherwise
        */
        uint8_t row_data;
        if(is_on)
            row_data = ((x == 0) ? 0x80 : (1 << (x - 1))) | display->buffer[y].data;
        else
            row_data = ((x == 0) ? 0x80 : (1 << (x - 1))) ^ display->buffer[y].data;

        /* 
            Check if the new data of the row is different than in the buffer and of it is, 
            change the value in the buffer and set boolean value indicating a change in row data to true
        */
        if(display->buffer[y].data != row_data)
        {
            display->buffer[y].data = row_data;
            display->buffer[y].has_changed = true;
        }
    }
}

// Sets the values (on/off : 1/0) of multiple pixels on the matrix display
void matrix_display_set_pixels(matrix_display_t* display, matrix_display_value_pair_t* pixel_values, unsigned int length)
{
    // Check if matrix display is initialized
    if(display->is_initialized)
    {
        // Loop trough all pixel value pairs given and set corresponding LED values in the buffer
        for(int i = 0; i < length; i++)
            matrix_display_set_pixel(display, pixel_values[i].x, pixel_values[i].y, pixel_values[i].is_on);
    }
}

// Updates the matrix display with the data in the buffer
void matrix_display_update(matrix_display_t* display)
{
    // Check if matrix display is initialized
    if(display->is_initialized)
    {
     	// Loop through all the rows in the buffer
        for(int y = 0; y < 8; y++)
        {
            // Check if row data has changes otherwise don't bother setting the register
            if(display->buffer[y].has_changed)
            {
                i2c_driver_write_register8(display->i2c_address, y * 2, display->buffer[y].data);	// Write row data to register
                display->buffer[y].has_changed = false;		// Set the changed state to false to indicate that the display is now in the correct state
            }
        }   
    }
}

// Sets the values of all the pixels of the matrix display given to the function to (off : 0) essentially clearing the matrix display
void matrix_display_clear(matrix_display_t* display)
{
    // Check if matrix display is initialized
    if(display->is_initialized)
    {
        // Loop trough all rows of the matrix and turn off the corresponding LED's
        for(int y = 0; y < 8; y++)
        {
            if(display->buffer[y].data != 0x00)
            {
                display->buffer[y].data = 0x00;										// Clear buffer row data
                display->buffer[y].has_changed = false;								// Clear buffer changed state
                i2c_driver_write_register8(display->i2c_address, y * 2, 0x00);		// Write 0 to row register to turn off LED's on the row
            }
        }
    }
}