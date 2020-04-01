#include "include/pipelane.h"

// Draws the pipelane given to the function on the matrix array given to the function
void pipelane_draw(pipelane_t** pipelane, matrix_array_t** matrix_array)
{
    // Loop through all possible y positions on the matrix array (array is setup vertically with two displays)
    for(int y = 0; y < 16; y++)
    {
        // Checks if the y coordinate is part of a pipe or the opening and sets the pixel on the matrix array to (true : on : 1)
        if(y < (int)((*pipelane)->openingYPosition - ((*pipelane)->openingSize / 2)) 
            || y > (int)((*pipelane)->openingYPosition + ((*pipelane)->openingSize / 2)))
        matrix_array_set_pixel(matrix_array, (int)(*pipelane)->xPosition, y, true);     // Set pixel value on matrix array
    }
}

// Updates the position of the pipelane
void pipelane_update(pipelane_t** pipelane, float xVelocity, float deltatime)
{
    (*pipelane)->xPosition += xVelocity * deltatime;    // Update x position with the x velocity and change in time since last update
}

// Checks if the x and y positions given to the function collides with a pipe on the pipelane
bool pipelane_check_collsion(pipelane_t** pipelane, float x, float y)
{
    // Check if the x position on the matrix is the same as given to the function
    if((int)((*pipelane)->xPosition) == (int)x)
    {
        // Check if the y position given to the function is on a pipe part and thus is colliding
        if(((int)y < (int)((*pipelane)->openingYPosition - ((*pipelane)->openingSize / 2)) 
            || (int)y > (int)((*pipelane)->openingYPosition + ((*pipelane)->openingSize / 2))))
            return true;
    }
    return false;
}

// Checks if the x and y positions given to the function is inside the opening of the pipelane
bool pipelane_check_opening(pipelane_t** pipelane, float x, float y)
{
        // Check if the x position on the matrix is the same as given to the function
    if((int)((*pipelane)->xPosition) == (int)x)
    {
        // Check if the y position given to the function is on a pipe part and thus is colliding
        if(!((int)y < (int)((*pipelane)->openingYPosition - ((*pipelane)->openingSize / 2)) 
            || (int)y > (int)((*pipelane)->openingYPosition + ((*pipelane)->openingSize / 2))))
            return true;
    }
    return false;
}