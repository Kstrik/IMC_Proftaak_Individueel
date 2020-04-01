/*
    Author: Kenley Strik
    Addition: This whole file was written by Kenley Strik
*/

#include "flappy_bird.h"

static bird_t* bird = NULL;
static pipelane_t* pipelane1 = NULL;
static pipelane_t* pipelane2 = NULL;
static matrix_array_t* matrix_array = NULL;
static gpio_button_t* gpio_button = NULL;
static buzzer_t* buzzer = NULL;

static bool is_playing = false;
static bool is_ready = false;
static bool first_flap = true;
static TimerHandle_t timer_handle;

static sequence_segment_t* score_sequence = NULL;
static sequence_segment_t* fail_sequence = NULL;

void flappy_bird_setup();
void flappy_bird_update();
void flappy_bird_set_random_opening_position(pipelane_t** pipelane, int upper, int lower);
void flappy_bird_timer_callback(TimerHandle_t xTimer);

// Initializes the flappy bird game
void flappy_bird_init()
{
    // Check if the flappy bird game is ready to be played and if not initialize all essentials
    if(!is_ready)
    {
        bird = (bird_t*)malloc(sizeof(bird_t));                             // Allocate memory for the bird
        pipelane1 = (pipelane_t*)malloc(sizeof(pipelane_t));                // Allocate memory for the first pipelane
        pipelane2 = (pipelane_t*)malloc(sizeof(pipelane_t));                // Allocate memory for the second pipelane

        matrix_array = (matrix_array_t*)malloc(sizeof(matrix_array_t));     // Allocate memory for the matrix array
        matrix_array->is_initialized = false;
        matrix_array_init(&matrix_array, VERTICAL);                         // Initialize the matrix array in vertical orientation
        matrix_array_add_matrix_display(&matrix_array, 0x70);               // Add first matrix diaply to the matrix array
        matrix_array_add_matrix_display(&matrix_array, 0x71);               // Add second matrix diaply to the matrix array

        gpio_button = (gpio_button_t*)malloc(sizeof(gpio_button_t));        // Allocate memory for the gpio button
        gpio_button->gpio_pin = 19;
        gpio_button->onButtonPressed = &flappy_bird_flap;
        
        buzzer = (buzzer_t*)malloc(sizeof(buzzer_t));                       // Allocate memory for the buzzer
        buzzer->gpio_pin = 18;
        buzzer->is_initialized = false;
        buzzer_init(&buzzer);                                               // Initialize the buzzer

        score_sequence = (sequence_segment_t*)malloc(sizeof(sequence_segment_t) * 3);   // Allocate memory for the score sequence
        score_sequence[0].frequency = 200;
        score_sequence[0].duration = 200;
        score_sequence[1].frequency = 400;
        score_sequence[1].duration = 200;
        score_sequence[2].frequency = 600;
        score_sequence[2].duration = 200;

        fail_sequence = (sequence_segment_t*)malloc(sizeof(sequence_segment_t) * 4);    // Allocate memory for the fail sequence
        fail_sequence[0].frequency = 600;
        fail_sequence[0].duration = 500;
        fail_sequence[1].frequency = 500;
        fail_sequence[1].duration = 500;
        fail_sequence[2].frequency = 300;
        fail_sequence[2].duration = 500;
        fail_sequence[3].frequency = 100;
        fail_sequence[3].duration = 500;

        srand(time(0));     // Set random seed
        is_ready = true;    // Set flappy bird game state to ready
    }
}

// Deinitializes the flappy bird game
void flappy_bird_deinit()
{
    // Check if the flappy bird game is ready to be played and is not playing
    if(is_ready && !is_playing)
    {
        free(bird);                             // Free memory of the bird
        free(pipelane1);                        // Free memory of the first pipelane
        free(pipelane2);                        // Free memory of the desocnd pipelane

        matrix_array_deinit(&matrix_array);     // Uninitialize matrix array
        buzzer_deinit(&buzzer);                 // Uninitialize buzzer
        free(matrix_array);                     // Free memory of matrix array
        free(gpio_button);                      // Free memory of the gpio_button
        free(buzzer);                           // Free memory of the buzzeer

        free(score_sequence);                   // Free memory of the score_sequence
        free(fail_sequence);                    // Free memory of the fail_sequence
        is_ready = false;                       // Set flappy bird game state to not ready
    }
}

// Start the flappy bird game and the task that updates the game
void flappy_bird_start()
{
    // Check if the flappy bird game is ready to be played and is not playing
    if(is_ready && !is_playing)
    {
        flappy_bird_setup();                            // Setup flappy bird game
        is_playing = true;
        // Create timer for updating the game
        timer_handle = xTimerCreate("flappy_bird_timer", pdMS_TO_TICKS(10), pdTRUE, NULL, &flappy_bird_timer_callback);
        xTimerStart(timer_handle, 10);                  // Start the timer for updating the game
        gpio_button_start_listener(&gpio_button);       // Start listening for button input
    }
}

// Stops the flappy bird game and the task that updates the game
void flappy_bird_stop()
{
    // Check if the flappy bird game is playing
    if(is_playing)
    {
        is_playing = false;
        xTimerStop(timer_handle, 10);                   // Stop the timer for updating the game
        xTimerDelete(timer_handle, 10);                 // Delete the timer for updating the game
        gpio_button_stop_stop_listener(&gpio_button);   // Stop listening for button input
    }
}

// Propels the bird upwards by setting a certain velocity in the opposite direction as gravity 
void flappy_bird_flap()
{
    // Check if the flappy bird game is playing
    if(is_playing)
        bird->yVelocity = -0.8f;

    // Check if the user has pressed the button for the fisrt time to start the round
    if(first_flap)
        first_flap = false;
}

// Sets up the flappy bird game and set the propperties of the bird and pipelanes
void flappy_bird_setup()
{    
    bird->xPosition = 2.0f;
    bird->yPosition = 7.0f;
    bird->yVelocity = -1.0f;
    bird->yAcceleration = 0.1f;

    pipelane1->xPosition = 10.0f;
    pipelane1->openingYPosition = 8.0f;
    pipelane1->openingSize = 4.0f;

    pipelane2->xPosition = 16.0f;
    pipelane2->openingYPosition = 8.0f;
    pipelane2->openingSize = 4.0f;

    flappy_bird_set_random_opening_position(&pipelane1, 12, 3);
    flappy_bird_set_random_opening_position(&pipelane2, 12, 3);

    first_flap = true;      // Reset first_flap to true to wait for updating when button is not yet pressed
}

// Updates the flappy bird game by updateing the bird and pipelanes and drawing them on the matrix array
void flappy_bird_update()
{
    if(is_playing)
    {
        matrix_array_clear(&matrix_array);          // Clear the matrix array

        // Check if game can update if the button was pressed one time
        if(!first_flap)
        {
            pipelane_update(&pipelane1, -0.2f, pdMS_TO_TICKS(10));      // Update the first pipelane
            pipelane_update(&pipelane2, -0.2f, pdMS_TO_TICKS(10));      // Update the second pipelane
            bird_update(&bird, pdMS_TO_TICKS(10));

            // Check if the pipelanes are off the left side of the matrix array and if so reset there position zo the pipelanes look infinite
            if(pipelane1->xPosition < 0)
            {
                pipelane1->xPosition = pipelane2->xPosition + 6.0f;
                flappy_bird_set_random_opening_position(&pipelane1, 12, 3);
            }
            else if(pipelane2->xPosition < 0)
            {
                pipelane2->xPosition = pipelane1->xPosition + 6.0f;
                flappy_bird_set_random_opening_position(&pipelane2, 12, 3);
            }
        }

        pipelane_draw(&pipelane1, &matrix_array);   // Draw the first pipelane on the matrix array
        pipelane_draw(&pipelane2, &matrix_array);   // Draw the second pipelane on the matrix array
        bird_draw(&bird, &matrix_array);            // Draw the bird on the matrix arraya

        matrix_array_update(&matrix_array);         // Update the matrix array with the values in the matrix displays

        // Checki if the bird is colliding with either a pipelane, the ceiling or the ground
        if(pipelane_check_collsion(&pipelane1, bird->xPosition, bird->yPosition) ||
                pipelane_check_collsion(&pipelane2, bird->xPosition, bird->yPosition) || bird->yPosition < 0 || bird->yPosition > 17)
        {
            buzzer->sequence = fail_sequence;   // Set current sequence of the buzzer to the fail sequence sound
            buzzer->segments_count = 4;         // Set length of sequence to the length of the fail sequence sound
            flappy_bird_setup();                // Reset the flappy bird game after colliding with either a pipelane, the ceiling or the ground   
            buzzer_play_sequence(&buzzer);      // Play fail sequence
        }

        if(pipelane_check_opening(&pipelane1, bird->xPosition, bird->yPosition) ||
                pipelane_check_opening(&pipelane2, bird->xPosition, bird->yPosition))
        {
            buzzer->sequence = score_sequence;  // Set current sequence of the buzzer to the score sequence sound
            buzzer->segments_count = 3;         // Set length of sequence to the length of the score sequence sound
            buzzer_play_sequence(&buzzer);      // Play score sequence
        }
    }
}

// Set the opening of the pipeline given to the function to a random y position between [lower] and [upper]
void flappy_bird_set_random_opening_position(pipelane_t** pipelane, int upper, int lower)
{
    int num = (rand() % (upper - lower + 1)) + lower;   // Generate random integer
    (*pipelane)->openingYPosition = (float)num;         // Set opening y position
}

// Callback function for the flappy bird game calling the update method for the flappy bird game
void flappy_bird_timer_callback(TimerHandle_t xTimer)
{
    flappy_bird_update();   // Update the flappy bird game
}