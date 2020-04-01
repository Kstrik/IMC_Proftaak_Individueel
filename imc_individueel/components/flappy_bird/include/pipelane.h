/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#ifndef PIPELANE_H
#define PIPELANE_H

#include "matrix_array.h"

#ifdef __cplusplus
extern "C" {
#endif

// Type representing a pipelane with its x position, the position of the opening for the bird and the size of the opening
typedef struct
{
    float xPosition;            // X position of the pipelane
    float openingYPosition;     // Y position of the opening where a bird can fly through
    float openingSize;          // Size of the opening for the bird
} pipelane_t;

// Draws the pipelane given to the function on the matrix array given to the function
void pipelane_draw(pipelane_t** pipelane, matrix_array_t** matrix_array);
// Updates the position of the pipelane
void pipelane_update(pipelane_t** pipelane, float xVelocity, float deltatime);
// Checks if the x and y positions given to the function collides with a pipe on the pipelane
bool pipelane_check_collsion(pipelane_t** pipelane, float x, float y);
// Checks if the x and y positions given to the function is inside the opening of the pipelane
bool pipelane_check_opening(pipelane_t** pipelane, float x, float y);

#ifdef __cplusplus
}
#endif

#endif  // PIPELANE_H