
#ifndef PANEL_H
#define PANEL_H

#include "main.h"
#include "modbus_motor.h"
#include "control_system.h"
#include "types.h"
#include <cstring>

typedef enum {
    SPEED_LOW,
    SPEED_MID,
    SPEED_HIGH,
    SPEED_UNDEFINED
} ManualSpeed;

typedef struct {
    u8 mode_button;
    u8 cw_button;
    u8 ccw_button;
    u8 low_speed;
    u8 high_speed;
} ButtonStates;

typedef enum {
    MODE_AUTO,
    MODE_MANUAL,
    MODE_UNDEFINED
} OperationMode;

typedef enum {
    DIR_FORWARD,
    DIR_REVERSE,
    DIR_STOP
} Direction;

OperationMode currentMode = MODE_UNDEFINED;
ManualSpeed currentSpeed = SPEED_UNDEFINED;
Direction currentDirection = DIR_STOP;
u8 isNewDataReady = 0;

inline ButtonStates ReadButtons(void) {
    ButtonStates states;
    
    // Read all buttons (high when pressed)
    states.mode_button = HAL_GPIO_ReadPin(GPIOC, MODE_IN_Pin);
    states.cw_button = HAL_GPIO_ReadPin(GPIOC, CW_IN_Pin);
    states.ccw_button = HAL_GPIO_ReadPin(GPIOC, CCW_IN_Pin);
    states.low_speed = HAL_GPIO_ReadPin(GPIOC, SPEED_LOW_IN_Pin);
    states.high_speed = HAL_GPIO_ReadPin(GPIOC, SPEED_HIGH_IN_Pin);
    
    return states;
}

#define DEBOUNCE_DELAY 50 

inline ButtonStates ReadButtons_Debounced(void) {
    ButtonStates states;
    ButtonStates tempStates;
    
    tempStates = ReadButtons();
    
    // Wait for debounce
    HAL_Delay(DEBOUNCE_DELAY);
    
    states = ReadButtons();
    
    // Button is considered pressed only if both readings show it pressed
    states.mode_button &= tempStates.mode_button;
    states.cw_button &= tempStates.cw_button;
    states.ccw_button &= tempStates.ccw_button;
    states.low_speed &= tempStates.low_speed;
    states.high_speed &= tempStates.high_speed;
    return states;
}

inline u8 UpdateOperationMode(void) {
    ButtonStates buttonStates = ReadButtons_Debounced();
    
    if (buttonStates.mode_button) {
        currentMode = MODE_AUTO;
    } else {
        currentMode = MODE_MANUAL;
    } 
    return 0;
}

inline u8 UpdateManualControls(void) {    
    if (currentMode == MODE_MANUAL) {
        ButtonStates buttonStates = ReadButtons_Debounced();
        
        // Speed logic - only triggers update when changed
        if (buttonStates.low_speed && buttonStates.high_speed) {
            currentSpeed = SPEED_MID;
        }
        else if (buttonStates.low_speed) {
            currentSpeed = SPEED_LOW;
        }
        else if (buttonStates.high_speed) {
            currentSpeed = SPEED_HIGH;
        }
        else {
            currentSpeed = SPEED_UNDEFINED;
        }
        
        if (buttonStates.cw_button && buttonStates.ccw_button) {
            currentDirection = DIR_STOP;
        }
        else if (buttonStates.cw_button) {
            currentDirection = DIR_FORWARD;
        } 
        else if (buttonStates.ccw_button) {
            currentDirection = DIR_REVERSE;
        } 
        else {
            currentDirection = DIR_STOP;
        }
    }
    return 0;
}



inline u8 UpdateAutoControls(void) {    
    if (currentMode == MODE_AUTO) {
        ButtonStates buttonStates = ReadButtons_Debounced();
        
        if (buttonStates.cw_button && buttonStates.ccw_button) {
            currentDirection = DIR_STOP;
        }
        else if (buttonStates.cw_button) {
            currentDirection = DIR_FORWARD;
        } 
        else if (buttonStates.ccw_button) {
            currentDirection = DIR_REVERSE;
        } 
        else {
            currentDirection = DIR_STOP;
        }
    }
    return 0;
}

inline void ControlSystem(void) {
    
    switch (currentMode) {
        
        
        case MODE_AUTO:
        
        Motor_Excitation_ON(DRUM_MOTOR_ID);
        HAL_Delay(DELAY_BETWEEN_COMMAND);
        Motor_Excitation_ON(SPOOLER_MOTOR_ID);
        HAL_Delay(DELAY_BETWEEN_COMMAND);
        
        if (currentDirection == DIR_FORWARD) {
            Pay_Out_control();
            
        } else if (currentDirection == DIR_REVERSE) {
            Pay_In_control();
        }
        
        else
        {
            Motor_Stop(DRUM_MOTOR_ID);
            HAL_Delay(DELAY_BETWEEN_COMMAND);
            Motor_Stop(SPOOLER_MOTOR_ID);
            HAL_Delay(DELAY_BETWEEN_COMMAND);
        }
        
        break;
        
        case MODE_MANUAL:
        
        
        // Motor excitation should be on to perform other motor control modes
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
                
                case SPEED_MID:
                if (currentDirection == DIR_FORWARD) {
                    Motor_CW_Mid_Speed(DRUM_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                    Motor_CW_Mid_Speed(SPOOLER_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                } else if (currentDirection == DIR_REVERSE) {
                    Motor_CCW_Mid_Speed(DRUM_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                    Motor_CCW_Mid_Speed(SPOOLER_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                }
                break;
                
                case SPEED_HIGH:
                if (currentDirection == DIR_FORWARD) {
                    Motor_CW_High_Speed(DRUM_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                    Motor_CW_High_Speed(SPOOLER_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                } else if (currentDirection == DIR_REVERSE) {
                    Motor_CCW_High_Speed(DRUM_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                    Motor_CCW_High_Speed(SPOOLER_MOTOR_ID);
                    HAL_Delay(DELAY_BETWEEN_COMMAND);
                }
                
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
    
    
}

#endif //PANEL_H