#include "include/bird.h"

// Draws the bird given to the function on the matrix array given to the function
void bird_draw(bird_t** bird, matrix_array_t** matrix_array)
{
    // Set pixel on the matrix array cooresponding to the x and y position of the bird to (true : on : 1)
    matrix_array_set_pixel(matrix_array, (int)(*bird)->xPosition, (int)(*bird)->yPosition, true);
}

// Updates the position and velocity of the bird
void bird_update(bird_t** bird, float deltatime)
{
    (*bird)->yVelocity += (*bird)->yAcceleration * deltatime;   // Update y velocity with the y accelaration and change in time since last update
    (*bird)->yPosition += (*bird)->yVelocity;                   // Update y position with the y velocity
}