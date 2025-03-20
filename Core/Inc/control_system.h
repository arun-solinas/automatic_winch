
/* March 8th 2025 8:02 pm */
#ifndef CONTROL_SYSTEM_H
#define CONTROL_SYSTEM_H

#include "main.h"
#include "ads1115_ads1015.h"


extern I2C_HandleTypeDef hi2c3;
ADS1xx5_I2C i2c;

float PID_loop(void) 
{
    static float tension_setpoint = 0.0f; 
    static float error = 0.0f;
    static float last_error = 0.0f;
    static float integral = 0.0f;
    static float derivative = 0.0f;
    static float Kp = 1.0f;  
    static float Ki = 0.0f;  
    static float Kd = 0.001f;  
    static float motor_speed = 0.0f;
    static float max_tension = 80.0f;
    
    
    // Filter variables
    static float filtered_tension = 0.0f;
    static float alpha = 0.2f; // Filter coefficient (0.0-1.0) - lower = more filtering
    
    
    ADS1115(&i2c, &hi2c3, ADS_ADDR_GND); 
    ADSsetGain(&i2c, GAIN_FOUR); 
    
    
    i32 raw_tension = ADSreadADC_Differential_0_1(&i2c);
    
    
    filtered_tension = alpha * raw_tension + (1.0f - alpha) * filtered_tension;
    i32 Tension = filtered_tension;
    
    
    error = Tension - tension_setpoint;
    
    
    integral += error;
    derivative = error - last_error;
    
    
    if(integral > 1000) integral = 1000;
    if(integral < -1000) integral = -1000; 
    
    
    motor_speed = Kp * error + Ki * integral + Kd * derivative;
    
    
    if(motor_speed < 0.0f) {
        motor_speed = 0;
    }
    
    if(motor_speed > max_tension) motor_speed = max_tension; 
    
    
    last_error = error;
    
    return motor_speed;
}


void Pay_Out_control(void) 
{
    float set_speed = PID_loop();
    r32 motor_speed = 0;
    
    
    if (set_speed >= 5.0f) {
        
        motor_speed = (set_speed - 5.0f) * (3000.0f / (100.0f - 5.0f));
    }
    
    Motor_Speed_Control(DRUM_MOTOR_ID, motor_speed);
    HAL_Delay(DELAY_BETWEEN_COMMAND);
    Motor_Speed_Control(SPOOLER_MOTOR_ID, motor_speed/4);
    HAL_Delay(DELAY_BETWEEN_COMMAND);
}


void Pay_In_control(void) {
    float set_speed = PID_loop();
    
    
    int32_t motor_speed;
    
    if (set_speed < 30) {
        
        motor_speed = -1 *  (int32_t)(3000 * (1.0f - set_speed / 100.0f));
    } else {
        
        motor_speed = 0;
    }
    
    
    Motor_Speed_Control(DRUM_MOTOR_ID, motor_speed);
    HAL_Delay(DELAY_BETWEEN_COMMAND);
    
    
    Motor_Speed_Control(SPOOLER_MOTOR_ID, motor_speed / 4);
    HAL_Delay(DELAY_BETWEEN_COMMAND);
}

#endif //CONTROL_SYSTEM_H