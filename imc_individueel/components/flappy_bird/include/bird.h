/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#ifndef BIRD_H
#define BIRD_H

#include "matrix_array.h"

#ifdef __cplusplus
extern "C" {
#endif

// Type representing a bird with its position, velocity and its downwards accelaration
typedef struct
{
    float xPosition, yPosition;     // X and Y position of the bird
    float yVelocity;                // Velocity of the bird on the Y axis
    float yAcceleration;            // Acceleration of the bird on the Y axis
} bird_t;

// Draws the bird given to the function on the matrix array given to the function
void bird_draw(bird_t** bird, matrix_array_t** matrix_array);
// Updates the position and velocity of the bird
void bird_update(bird_t** bird, float deltatime);

#ifdef __cplusplus
}
#endif

#endif  // BIRD_H