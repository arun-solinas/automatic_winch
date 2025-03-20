/* date = January 4th 2025 10:14 pm */


#ifndef ENCODER_H
#define ENCODER_H

#include "main.h"
#include "types.h"
#include <stdint.h>
#include <cmath>
#include <stdio.h>


#define ENCODER_PPR 4000.0f         // Pulses Per Revolution
#define PULLEY_DIAMETER 50.0f       // Pulley diameter in mm
#define TIMER_INTERVAL_MS 100       // Calculation interval in milliseconds
#define MAX_BUFFER_SIZE 100


extern TIM_HandleTypeDef htim2;     //FROM HAL
extern UART_HandleTypeDef huart2;

typedef enum {
    WIND_DIRECTION,      // Positive rotation (increasing length)
    UNWIND_DIRECTION     // Negative rotation (decreasing length)
} RotationDirection;



global_variable r32 cable_length;            // Total cable length in meters
global_variable r32 cable_speed;             // Cable speed in meters per second
global_variable i32 encoder_count;              // Current encoder count
global_variable i32 last_encoder_count;         // Previous encoder count


RotationDirection current_direction = WIND_DIRECTION;



// NOTE(ARUN): Determine Rotation Direction
function inline 
RotationDirection determine_rotation_direction(i32 encoder_delta)
{
    return (encoder_delta > 0) ? WIND_DIRECTION : UNWIND_DIRECTION;
}

// NOTE(ARUN): Data transfer to endobot app

function inline
void send_cable_info(void){
    char uart_buffer[MAX_BUFFER_SIZE];
    u16 winch_data;
    winch_data = sprintf(uart_buffer, "ENC:%ld,LEN:%.3f,SPD:%.3f\r\n", 
                         (long)encoder_count, 
                         (float)cable_length, 
                         (float)cable_speed);
    HAL_UART_Transmit(&huart2, (u8*)uart_buffer, winch_data, HAL_MAX_DELAY);
}


function inline 
void calculate_cable_metrics(void)
{
    // Calculate circumference of the pulley (in meters)
    r32 pulley_circumference = (PI * PULLEY_DIAMETER) / 1000.0f;  // Convert mm to m
    
    // Calculate distance per pulse
    r32 distance_per_pulse = pulley_circumference / ENCODER_PPR;
    
    // Get current encoder count
    encoder_count = __HAL_TIM_GET_COUNTER(&htim2);
    
    // Calculate encoder count delta
    i32 encoder_delta = encoder_count - last_encoder_count;
    
    // Handle 16-bit timer overflow
    if (encoder_delta < -32768)
    {
        encoder_delta += 65536;
    }
    else if (encoder_delta > 32767)
    {
        encoder_delta -= 65536;
    }
    
    
    current_direction = determine_rotation_direction(encoder_delta);
    
    r32 incremental_length = fabsf(encoder_delta * distance_per_pulse);
    
    if (current_direction == WIND_DIRECTION) {
        
        cable_length += incremental_length;
    } else {
        
        cable_length = fmaxf(0.0f, cable_length - incremental_length);
    }
    
    cable_speed = incremental_length / (TIMER_INTERVAL_MS / 1000.0f);
    
    last_encoder_count = encoder_count;
    
    send_cable_info();
    
    
}


// NOTE(ARUN): Initialization function

function inline 
void cable_metrics_init(void)
{
    // Initialize encoder count
    last_encoder_count = __HAL_TIM_GET_COUNTER(&htim2);
    
    // Reset metrics
    cable_length = 0.0f;
    cable_speed = 0.0f;
    current_direction = WIND_DIRECTION;
}


#endif // ENCODER_H

