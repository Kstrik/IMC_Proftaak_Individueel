#ifndef BUZZER_H
#define BUZZER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "driver/mcpwm.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Type representing a segment of a sequency of sounds
typedef struct
{
    unsigned int frequency;             // Frequency of the sequence segment the buzzer needs to vibrate in
    unsigned int duration;              // Duration of the the buzzer needs to make the specified frequency
} sequence_segment_t;

// Type representing a buzzer connected to a GPIO pin
typedef struct
{
    uint8_t gpio_pin;                       // GPIO pin where the buzzer is connected to
    bool is_initialized;                    // Boolean value indicating if the buzzer is initalized
    bool is_playing;                        // Boolean valie indicating if the buzzer is playing
    
    unsigned int current_interval;          // The current interval in milliseconds the task had to wait before stopping the sound

    sequence_segment_t* sequence;             // Currently active sequence
    unsigned int current_segment_index;     // Current index of the segment playing
    unsigned int segments_count;            // Ammount of segments in de active sequence
} buzzer_t;

// Initializes the buzzer given to the function
void buzzer_init(buzzer_t** buzzer);
// Deinitializes the buzzer given to the function
void buzzer_deinit(buzzer_t** buzzer);

// Sets the frequency of the buzzer given tot the function and starts a task that waits the specified ammount of duration in milliseconds
void buzzer_play_frequency(buzzer_t** buzzer, unsigned int frequency, unsigned int duration);
// Starts a task that plays all the sequence segments one by one
void buzzer_play_sequence(buzzer_t** buzzer);

#ifdef __cplusplus
}
#endif

#endif  // BUZZER_H