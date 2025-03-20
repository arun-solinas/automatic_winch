#ifndef MODBUS_MOTOR_H
#define MODBUS_MOTOR_H

#include "main.h"
#include "types.h"

// NOTE(ARUN): Function Codes

#define MODBUS_READ_HOLDING_REGS    0x03
#define MODBUS_WRITE_SINGLE_REG     0x06
#define MODBUS_WRITE_MULTIPLE_REGS  0x10

// NOTE(ARUN): Operation Types
#define CONTINUOUS_OPERATION_SPEED_CONTROL  0x0030

// NOTE(ARUN): Register Addresses
#define REG_OPERATION_START    0x005A
#define REG_EXCITATION        0x007C

// NOTE(ARUN): Timing Parameters (milliseconds)
#define MODBUS_RESPONSE_TIMEOUT     1000
#define MODBUS_INTER_FRAME_DELAY    100
#define MODBUS_RETRY_COUNT          3
#define MODBUS_COMMAND_DELAY        50
#define MODBUS_ERROR_RECOVERY_TIME  200

// NOTE(ARUN): Motor IDs
#define DRUM_MOTOR_ID      1
#define SPOOLER_MOTOR_ID   2


// NOTE(ARUN): Drum Motor Speed Levels (RPM)
#define M1_SPEED_LOW      800    // 500 match with endoto 22omm wheel speedd 20
#define M1_SPEED_MID      1200
#define M1_SPEED_HIGH     3000  // 1500 match with endoto 220mm wheel 60 speed

// NOTE(ARUN): Spooler Motor Speed Levels (RPM)
#define M2_SPEED_LOW      (M1_SPEED_LOW/4)
#define M2_SPEED_MID      (M1_SPEED_MID/4)
#define M2_SPEED_HIGH     (M1_SPEED_HIGH/4)

// NOTE(ARUN): Motor Parameters
#define DEFAULT_ACCELERATION    1000    // 1000ms
#define DEFAULT_DECELERATION    1000    // 1000ms
#define DEFAULT_TORQUE_LIMIT    900   // 25.00%


// NOTE(ARUN): send commmand delay
#define DELAY_BETWEEN_COMMAND 8  //MS

// NOTE(ARUN): Error Codes
#define MODBUS_OK             0
#define MODBUS_ERROR_CRC      1
#define MODBUS_ERROR_TIMEOUT  2
#define MODBUS_ERROR_INVALID  3
#define MODBUS_ERROR_BUSY     4
#define MODBUS_ERROR_SEQUENCE 5




// NOTE(ARUN): Modbus State Structure
typedef struct {
    u32 lastCommandTime;
    u8 busyFlag;
    u8 retryCount;
    u8 lastSlaveID;
    u8 lastFunction;
    u16 lastRegister;
    u16 lastValue;
} ModbusState_t;

extern UART_HandleTypeDef huart2;

global_variable ModbusState_t modbusState;


// NOTE(ARUN): Core Functions Implementation

function inline
u8 Modbus_Init(void) {
    
    modbusState.lastCommandTime = 0;
    modbusState.busyFlag = 0;
    modbusState.retryCount = 0;
    modbusState.lastSlaveID = 0;
    modbusState.lastFunction = 0;
    modbusState.lastRegister = 0;
    modbusState.lastValue = 0;
    return MODBUS_OK;
}

function inline
u16 Modbus_CalculateCRC(u8 *buffer, u16 length) {
    u16 crc = 0xFFFF;
    for (u16 i = 0; i < length; i++) {
        crc ^= buffer[i];
        for (u8 j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}


// u8 modbus_send_command();

function inline 
u8 Modbus_SendCommand(u8 *request, u16 length) {
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, request, length, MODBUS_RESPONSE_TIMEOUT);
    return (status == HAL_OK) ? MODBUS_OK : MODBUS_ERROR_TIMEOUT;
}


function inline 
u8 Modbus_WaitResponse(uint32_t timeout) {
    u32 startTime = HAL_GetTick();
    while ((HAL_GetTick() - startTime) < timeout) {
        if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE)) {
            return MODBUS_OK;
        }
        HAL_Delay(1);
    }
    return MODBUS_ERROR_TIMEOUT;
}


// NOTE(ARUN): Motor Control Functions Implementation

function inline
b32 excite_motor(u8 motorID)
{
    u8 request[16] = {0};
    
    request[0] = motorID;           // Address
    request[1] = 0x10;             // Function code
    request[2] = 0x00;             // Register address high
    request[3] = 0x7C;             // Register address low
    request[4] = 0x00;             // Number of registers high
    request[5] = 0x02;             // Number of registers low
    request[6] = 0x04;             // Byte count
    request[7] = 0x00;             // Data high
    request[8] = 0x00;             // Data
    request[9] = 0x00;             // Data
    request[10] = 0x01;            // Data low (ON)
    
    u16 crc = Modbus_CalculateCRC(request, 11);
    request[11] = crc & 0xFF;
    request[12] = (crc >> 8) & 0xFF;
    
    return Modbus_SendCommand(request, 13);
}

function inline
u8 Motor_Excitation_ON(u8 motorID) {
    u8 request[16] = {0};
    
    request[0] = motorID;           // Address
    request[1] = 0x10;             // Function code
    request[2] = 0x00;             // Register address high
    request[3] = 0x7C;             // Register address low
    request[4] = 0x00;             // Number of registers high
    request[5] = 0x02;             // Number of registers low
    request[6] = 0x04;             // Byte count
    request[7] = 0x00;             // Data high
    request[8] = 0x00;             // Data
    request[9] = 0x00;             // Data
    request[10] = 0x01;            // Data low (ON)
    
    u16 crc = Modbus_CalculateCRC(request, 11);
    request[11] = crc & 0xFF;
    request[12] = (crc >> 8) & 0xFF;
    
    return Modbus_SendCommand(request, 13);
}

function inline
u8 Motor_Excitation_OFF(u8 motorID) {
    u8 request[16] = {0};
    
    request[0] = motorID;
    request[1] = 0x10;
    request[2] = 0x00;
    request[3] = 0x7C;
    request[4] = 0x00;
    request[5] = 0x02;
    request[6] = 0x04;
    request[7] = 0x00;
    request[8] = 0x00;
    request[9] = 0x00;
    request[10] = 0x00;            // OFF
    
    u16 crc = Modbus_CalculateCRC(request, 11);
    request[11] = crc & 0xFF;
    request[12] = (crc >> 8) & 0xFF;
    
    return Modbus_SendCommand(request, 13);
}


function inline
u8 Motor_Speed_Control(u8 motorID, i16 speed) {
    u8 request[40] = {0};
    
    request[0] = motorID;                // Address
    request[1] = 0x10;                   // Function code
    request[2] = 0x00;                   // Register address high
    request[3] = 0x5A;                   // Register address low
    request[4] = 0x00;                   // Number of registers high
    request[5] = 0x0E;                   // Number of registers low
    request[6] = 0x1C;                   // Byte count
    
    // Operation type (Continuous operation speed control)
    request[7] = 0x00;
    request[8] = 0x00;
    request[9] = 0x00;
    request[10] = 0x30;
    
    // Position (unused)
    request[11] = 0x00;
    request[12] = 0x00;
    request[13] = 0x00;
    request[14] = 0x00;
    
    // Speed
    if (speed >= 0) {
        // Positive values: 00 00 followed by value
        request[15] = 0x00;
        request[16] = 0x00;
        request[17] = (speed >> 8) & 0xFF;
        request[18] = speed & 0xFF;
    } else {
        
        i32 fullValue = speed;  
        request[15] = (fullValue >> 24) & 0xFF; 
        request[16] = (fullValue >> 16) & 0xFF; 
        request[17] = (fullValue >> 8) & 0xFF;   
        request[18] = fullValue & 0xFF;      
    }
    
    // Acceleration
    request[19] = 0x00;
    request[20] = 0x00;
    request[21] = (DEFAULT_ACCELERATION >> 8) & 0xFF;
    request[22] = DEFAULT_ACCELERATION & 0xFF;
    
    // Deceleration
    request[23] = 0x00;
    request[24] = 0x00;
    request[25] = (DEFAULT_DECELERATION >> 8) & 0xFF;
    request[26] = DEFAULT_DECELERATION & 0xFF;
    
    // Torque limit
    request[27] = 0x00;
    request[28] = 0x00;
    request[29] = (DEFAULT_TORQUE_LIMIT >> 8) & 0xFF;
    request[30] = DEFAULT_TORQUE_LIMIT & 0xFF;
    
    // Trigger
    request[31] = 0x00;
    request[32] = 0x00;
    request[33] = 0x00;
    request[34] = 0x01;
    
    u16 crc = Modbus_CalculateCRC(request, 35);
    request[35] = crc & 0xFF;
    request[36] = (crc >> 8) & 0xFF;
    
    return Modbus_SendCommand(request, 37);
}


// NOTE(rsb): CW Speed Control Functions

u8 Motor_CW_Low_Speed(uint8_t motorID) {
    u16 speed = (motorID == DRUM_MOTOR_ID) ? M1_SPEED_LOW : M2_SPEED_LOW;
    return Motor_Speed_Control(motorID, speed);
}

u8 Motor_CW_Mid_Speed(u8 motorID) {
    u16 speed = (motorID == DRUM_MOTOR_ID) ? M1_SPEED_MID : M2_SPEED_MID;
    return Motor_Speed_Control(motorID, speed);
}

u8 Motor_CW_High_Speed(u8 motorID) {
    u16 speed = (motorID == DRUM_MOTOR_ID) ? M1_SPEED_HIGH : M2_SPEED_HIGH;
    return Motor_Speed_Control(motorID, speed);
}

// CCW Speed Control Functions
u8 Motor_CCW_Low_Speed(u8 motorID) {
    u16 speed = (motorID == DRUM_MOTOR_ID) ? M1_SPEED_LOW : M2_SPEED_LOW;
    return Motor_Speed_Control(motorID, -speed);
}

u8 Motor_CCW_Mid_Speed(u8 motorID) {
    u16 speed = (motorID == DRUM_MOTOR_ID) ? M1_SPEED_MID : M2_SPEED_MID;
    return Motor_Speed_Control(motorID, -speed);
}

u8 Motor_CCW_High_Speed(u8 motorID) {
    u16 speed = (motorID == DRUM_MOTOR_ID) ? M1_SPEED_HIGH : M2_SPEED_HIGH;
    return Motor_Speed_Control(motorID, -speed);
}

u8 Motor_Stop(u8 motorID) {
    return Motor_Speed_Control(motorID, 0);
}






#endif // MODBUS_MOTOR_H
