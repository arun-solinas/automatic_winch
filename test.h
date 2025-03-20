/* date = March 7th 2025 6:36 pm */

#ifndef TEST_H
#define TEST_H
inline void ControlSystem(int16_t diff_0_1) {
    if (!isNewDataReady) {
        return;
    }
    
    switch (currentMode) {
        case MODE_AUTO:
        // Enable motor excitation for AUTO mode
        Motor_Excitation_ON(DRUM_MOTOR_ID);
        HAL_Delay(DELAY_BETWEEN_COMMAND);
        
        // Dynamic motor control based on differential reading
        if (diff_0_1 > 100) {  // Positive threshold
            // Calculate proportional speed based on diff_0_1 value
            // Map from ADC range to motor speed range
            u16 speed = (u16)((diff_0_1 * M1_SPEED_HIGH) / 16384);
            
            // Limit to valid speed range
            if (speed > M1_SPEED_HIGH) speed = M1_SPEED_HIGH;
            if (speed < M1_SPEED_LOW) speed = M1_SPEED_LOW;
            
            // Apply forward direction with calculated speed
            Motor_Speed_Control(DRUM_MOTOR_ID, speed);
        }
        else if (diff_0_1 < -100) {  // Negative threshold
            // Calculate proportional speed based on absolute diff_0_1 value
            // Map from ADC range to motor speed range
            u16 speed = (u16)((-diff_0_1 * M1_SPEED_HIGH) / 16384);
            
            // Limit to valid speed range
            if (speed > M1_SPEED_HIGH) speed = M1_SPEED_HIGH;
            if (speed < M1_SPEED_LOW) speed = M1_SPEED_LOW;
            
            // Apply reverse direction with calculated speed
            Motor_Speed_Control(DRUM_MOTOR_ID, -speed);
        }
        else {
            // In deadband zone, stop motor
            Motor_Stop(DRUM_MOTOR_ID);
        }
        
        HAL_Delay(DELAY_BETWEEN_COMMAND);
        
        // In AUTO mode, we'll keep the spooler motor off
        Motor_Excitation_OFF(SPOOLER_MOTOR_ID);
        HAL_Delay(DELAY_BETWEEN_COMMAND);
        break;
        
        case MODE_MANUAL:
        // Your existing manual mode code remains unchanged
        Motor_Excitation_ON(DRUM_MOTOR_ID);
        HAL_Delay(DELAY_BETWEEN_COMMAND);
        Motor_Excitation_ON(SPOOLER_MOTOR_ID);
        HAL_Delay(DELAY_BETWEEN_COMMAND);
        
        if (currentDirection != DIR_STOP) {
            switch (currentSpeed) {
                case SPEED_LOW:
                if (currentDirection == DIR_FORWARD) {
                    Motor_CW_Low_Speed(DRUM_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                    Motor_CW_Low_Speed(SPOOLER_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                } else if (currentDirection == DIR_REVERSE) {
                    Motor_CCW_Low_Speed(DRUM_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                    Motor_CCW_Low_Speed(SPOOLER_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                }
                break;
                
                // Rest of your existing manual control cases...
                case SPEED_MID:
                // ... existing code ...
                break;
                
                case SPEED_HIGH:
                // ... existing code ...
                break;
                
                default:
                Motor_Excitation_OFF(DRUM_MOTOR_ID);
                HAL_Delay(DELAY_BETWEEN_COMMAND);
                Motor_Excitation_OFF(SPOOLER_MOTOR_ID);
                HAL_Delay(DELAY_BETWEEN_COMMAND);
                break;
            }
        } else {
            Motor_Stop(DRUM_MOTOR_ID);
            HAL_Delay(DELAY_BETWEEN_COMMAND);
            Motor_Stop(SPOOLER_MOTOR_ID);
            HAL_Delay(DELAY_BETWEEN_COMMAND);
        }
        break;
        
        default:
        break;
    }
    
    isNewDataReady = 0;
}


#endif //TEST_H
