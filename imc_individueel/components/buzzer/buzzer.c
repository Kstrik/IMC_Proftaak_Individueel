#include "include/buzzer.h"

void buzzer_set_duty_cycle(buzzer_t** buzzer, unsigned int duty_cycle);
void buzzer_set_frequency(buzzer_t** buzzer, unsigned int frequency);
void buzzer_play_frequency_task(void* pvParameter);
void buzzer_play_sequence_task(void* pvParameter);

// Initializes the buzzer given to the function
void buzzer_init(buzzer_t** buzzer)
{
    // Check if buzzer is initalized and if not initialize it
    if(!(*buzzer)->is_initialized)
    {
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, (*buzzer)->gpio_pin);    // Initialize mcpwm on the specified GPIO pin

        mcpwm_config_t pwm_config;
        pwm_config.frequency = 50;
        pwm_config.cmpr_a = 0;
        pwm_config.counter_mode = MCPWM_UP_COUNTER;
        pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
        mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);           // Initialize mcpwm with the specified PWM configuration

        (*buzzer)->is_playing = false;                                  // Set boolean value indicating the buzzer is playing to false    
        (*buzzer)->is_initialized = true;                               // Set boolean value indicating the buzzer is initialize to true
        (*buzzer)->current_interval = 0;                                // Set current time interval of the playing frequency to 0
        (*buzzer)->current_segment_index = 0;                           // Set current index the segment playing in the sequence to first element
    }
}

// Deinitializes the buzzer given to the function
void buzzer_deinit(buzzer_t** buzzer)
{
    // Check if buzzer is initalized and if it is uninitialize it
    if((*buzzer)->is_initialized)
        (*buzzer)->is_initialized = false;                              // Set boolean value indicating the buzzer is initialize to false
}

// Sets the duty cycle on the GPIO pin the buzzer is connected to
void buzzer_set_duty_cycle(buzzer_t** buzzer, unsigned int duty_cycle)
{
    // Check if buzzer is initalized
    if((*buzzer)->is_initialized)
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_cycle);     // Set duty cycle
}

// Sets the frequency of the PWM signal for the buzzer
void buzzer_set_frequency(buzzer_t** buzzer, unsigned int frequency)
{
    // Check if buzzer is initalized
    if((*buzzer)->is_initialized)
        mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_0, frequency);                    // Set frequency
}

// Sets the frequency of the buzzer given tot the function and starts a task that waits the specified ammount of duration in milliseconds
void buzzer_play_frequency(buzzer_t** buzzer, unsigned int frequency, unsigned int duration)
{
    // Check if buzzer is initalized and is not playing
    if((*buzzer)->is_initialized && !(*buzzer)->is_playing)
    {
        (*buzzer)->is_playing = true;                   // Set boolean value indicating the buzzer is playing to true
        (*buzzer)->current_interval = duration;         // Set duration of the current frequency playing
        buzzer_set_frequency(buzzer, frequency);        // Set the frequency to the specified ammount
        buzzer_set_duty_cycle(buzzer, 200);             // Set the duty cycle to 200 to make the buzzer louder

        // Create task to play frequency for the specified duration
        xTaskCreate(&buzzer_play_frequency_task, "buzzer_frequency_task", 2048, buzzer, 5, NULL);
    }
}

// Starts a task that plays all the sequence segments one by one
void buzzer_play_sequence(buzzer_t** buzzer)
{
    // Check if buzzer is initalized and is not playing
    if((*buzzer)->is_initialized && !(*buzzer)->is_playing && (*buzzer)->sequence != NULL)
    {
        (*buzzer)->is_playing = true;                   // Set boolean value indicating the buzzer is playing to true
        (*buzzer)->current_segment_index = 0;           // Set current index the segment playing in the sequence
        buzzer_set_duty_cycle(buzzer, 200);             // Set the duty cycle to 200 to make the buzzer louder

        // Create task to play sequence
        xTaskCreate(&buzzer_play_sequence_task, "buzzer_sequence_task", 4048, buzzer, 5, NULL);
    }
}

// Function for a task to wait the specified ammount of duration and turn off the sound of the buzzer when done
void buzzer_play_frequency_task(void* pvParameter)
{
    buzzer_t** buzzer = (buzzer_t**)pvParameter;                    // Cast void pointer to buzzer pointer given as parameter to the task
    vTaskDelay((*buzzer)->current_interval / portTICK_RATE_MS);     // Wait the specified ammount of time before turing off teh sound
    
    (*buzzer)->is_playing = false;
    buzzer_set_duty_cycle(buzzer, 0);       // Set the duty cycle to 0 to stop the buzzer form making sounds
    buzzer_set_frequency(buzzer, 1);        // Set frequency to 1 to make sure you can't hear anything

    vTaskDelete(NULL);                      // Delete task
}

// Function for a task to exceute all segments in a sequence and wait the specified ammount of time for evry segment
void buzzer_play_sequence_task(void* pvParameter)
{
    buzzer_t** buzzer = (buzzer_t**)pvParameter;                    // Cast void pointer to buzzer pointer given as parameter to the task

    while(true)
    {
        sequence_segment_t* segment = &((*buzzer)->sequence[(*buzzer)->current_segment_index]);
        buzzer_set_frequency(buzzer, segment->frequency);           // Set the frequency to the specified ammount
        vTaskDelay(segment->duration / portTICK_RATE_MS);           // Delay task for specified duration of the current segment

        (*buzzer)->current_segment_index++;                         // Increment current segment index
        // Check if the sequence is finished
        if((*buzzer)->current_segment_index == (*buzzer)->segments_count)
            break;
    }

    (*buzzer)->current_segment_index = 0;   // Reset current segment index to 0 to start at the start next time
    (*buzzer)->is_playing = false;          // Set boolean value indicating the buzzer is playing to false
    buzzer_set_duty_cycle(buzzer, 0);       // Set the duty cycle to 0 to stop the buzzer form making sounds
    buzzer_set_frequency(buzzer, 1);        // Set frequency to 1 to make sure you can't hear anything
    
    vTaskDelete(NULL);                      // Delete task
}