/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#ifndef FLAPPY_BIRD_H
#define FLAPPY_BIRD_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include <stdlib.h>
#include <time.h>

#include "bird.h"
#include "pipelane.h"
#include "matrix_array.h"
#include "gpio_button.h"
#include "buzzer.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initializes the flappy bird game
void flappy_bird_init();
// Deinitializes the flappy bird game
void flappy_bird_deinit();

// Start the flappy bird game and the task that updates the game
void flappy_bird_start();
// Stops the flappy bird game and the task that updates the game
void flappy_bird_stop();
// Propels the bird upwards by setting a certain velocity in the opposite direction as gravity 
void flappy_bird_flap();

#ifdef __cplusplus
}
#endif

#endif  // FLAPPY_BIRD_H