/* date = January 4th 2025 11:02 am */

#ifndef ADS1115_ADS1015_H
#define ADS1115_ADS1015_H

#include "main.h"




// NOTE(rsb): I2C ADDRESS/BITS - Connect the following pin to ADDR

#define ADS_ADDR_GND (0x48) // 1001 000 (ADDR -> GND)
#define ADS_ADDR_VDD (0x49) // 1001 001 (ADDR -> VDD)
#define ADS_ADDR_SDA (0x4A) // 1001 010 (ADDR -> SDA)
#define ADS_ADDR_SCL (0x4B) // 1001 011 (ADDR -> SCL)


// NOTE(rsb): CONVERSION DELAY (in mS)


#define ADS1015_CONVERSIONDELAY (1) // Conversion delay
#define ADS1115_CONVERSIONDELAY (8) // Conversion delay



// NOTE(rsb): POINTER REGISTER


#define ADS1015_REG_POINTER_MASK         (0x03)   // Point mask
#define ADS1015_REG_POINTER_CONVERT      (0x00)   // Conversion
#define ADS1015_REG_POINTER_CONFIG       (0x01)   // Configuration
#define ADS1015_REG_POINTER_LOWTHRESH    (0x02)   // Low threshold
#define ADS1015_REG_POINTER_HITHRESH     (0x03)   // High threshold

// NOTE(rsb): CONFIG REGISTER

#define ADS1015_REG_CONFIG_OS_MASK      (0x8000) // OS Mask
#define ADS1015_REG_CONFIG_OS_SINGLE    (0x8000) // Write: Set to start a single-conversion
#define ADS1015_REG_CONFIG_OS_BUSY      (0x0000) // Read: Bit = 0 when conversion is in progress
#define ADS1015_REG_CONFIG_OS_NOTBUSY   (0x8000) // Read: Bit = 1 when device is not performing a conversion

#define ADS1015_REG_CONFIG_MUX_MASK     (0x7000) // Mux Mask
#define ADS1015_REG_CONFIG_MUX_DIFF_0_1 (0x0000) // Differential P = AIN0, N = AIN1 (default)
#define ADS1015_REG_CONFIG_MUX_DIFF_0_3 (0x1000) // Differential P = AIN0, N = AIN3
#define ADS1015_REG_CONFIG_MUX_DIFF_1_3 (0x2000) // Differential P = AIN1, N = AIN3
#define ADS1015_REG_CONFIG_MUX_DIFF_2_3 (0x3000) // Differential P = AIN2, N = AIN3
#define ADS1015_REG_CONFIG_MUX_SINGLE_0 (0x4000) // Single-ended AIN0
#define ADS1015_REG_CONFIG_MUX_SINGLE_1 (0x5000) // Single-ended AIN1
#define ADS1015_REG_CONFIG_MUX_SINGLE_2 (0x6000) // Single-ended AIN2
#define ADS1015_REG_CONFIG_MUX_SINGLE_3 (0x7000) // Single-ended AIN3

#define ADS1015_REG_CONFIG_PGA_MASK     (0x0E00) // PGA Mask
#define ADS1015_REG_CONFIG_PGA_6_144V   (0x0000) // +/-6.144V range = Gain 2/3
#define ADS1015_REG_CONFIG_PGA_4_096V   (0x0200) // +/-4.096V range = Gain 1
#define ADS1015_REG_CONFIG_PGA_2_048V   (0x0400) // +/-2.048V range = Gain 2 (default)
#define ADS1015_REG_CONFIG_PGA_1_024V   (0x0600) // +/-1.024V range = Gain 4
#define ADS1015_REG_CONFIG_PGA_0_512V   (0x0800) //  +/-0.512V range = Gain 8
#define ADS1015_REG_CONFIG_PGA_0_256V   (0x0A00) // +/-0.256V range = Gain 16

#define ADS1015_REG_CONFIG_MODE_MASK    (0x0100) // Mode Mask
#define ADS1015_REG_CONFIG_MODE_CONTIN  (0x0000) // Continuous conversion mode
#define ADS1015_REG_CONFIG_MODE_SINGLE  (0x0100) // Power-down single-shot mode (default)

#define ADS1015_REG_CONFIG_DR_MASK      (0x00E0) // Data Rate Mask
#define ADS1015_REG_CONFIG_DR_128SPS    (0x0000) // 128 samples per second
#define ADS1015_REG_CONFIG_DR_250SPS    (0x0020) // 250 samples per second
#define ADS1015_REG_CONFIG_DR_490SPS    (0x0040) // 490 samples per second
#define ADS1015_REG_CONFIG_DR_920SPS    (0x0060) // 920 samples per second
#define ADS1015_REG_CONFIG_DR_1600SPS   (0x0080) // 1600 samples per second (default)
#define ADS1015_REG_CONFIG_DR_2400SPS   (0x00A0) // 2400 samples per second
#define ADS1015_REG_CONFIG_DR_3300SPS   (0x00C0) // 3300 samples per second

#define ADS1015_REG_CONFIG_CMODE_MASK   (0x0010) // CMode Mask
#define ADS1015_REG_CONFIG_CMODE_TRAD   (0x0000) // Traditional comparator with hysteresis (default)
#define ADS1015_REG_CONFIG_CMODE_WINDOW (0x0010) // Window comparator

#define ADS1015_REG_CONFIG_CPOL_MASK    (0x0008) // CPol Mask
#define ADS1015_REG_CONFIG_CPOL_ACTVLOW (0x0000) // ALERT/RDY pin is low when active (default)
#define ADS1015_REG_CONFIG_CPOL_ACTVHI  (0x0008) // ALERT/RDY pin is high when active

#define ADS1015_REG_CONFIG_CLAT_MASK    (0x0004) // Determines if ALERT/RDY pin latches once asserted
#define ADS1015_REG_CONFIG_CLAT_NONLAT  (0x0000) // Non-latching comparator (default)
#define ADS1015_REG_CONFIG_CLAT_LATCH   (0x0004) // Latching comparator

#define ADS1015_REG_CONFIG_CQUE_MASK    (0x0003) // CQue Mask
#define ADS1015_REG_CONFIG_CQUE_1CONV   (0x0000) // Assert ALERT/RDY after one conversions
#define ADS1015_REG_CONFIG_CQUE_2CONV   (0x0001) // Assert ALERT/RDY after two conversions
#define ADS1015_REG_CONFIG_CQUE_4CONV   (0x0002) // Assert ALERT/RDY after four conversions
#define ADS1015_REG_CONFIG_CQUE_NONE    (0x0003) // Disable the comparator and put ALERT/RDY in high state (default)

// NOTE(rsb): Gain settings 

typedef enum {
    GAIN_TWOTHIRDS = ADS1015_REG_CONFIG_PGA_6_144V,
    GAIN_ONE = ADS1015_REG_CONFIG_PGA_4_096V,
    GAIN_TWO = ADS1015_REG_CONFIG_PGA_2_048V,
    GAIN_FOUR = ADS1015_REG_CONFIG_PGA_1_024V,
    GAIN_EIGHT = ADS1015_REG_CONFIG_PGA_0_512V,
    GAIN_SIXTEEN = ADS1015_REG_CONFIG_PGA_0_256V
} adsGain_t;


typedef struct {
	uint16_t m_i2cAddress;       // the I2C address
	uint32_t m_conversionDelay;  // conversion delay
	uint8_t m_bitShift;          // bit shift amount
	adsGain_t m_gain;            // ADC gain
	I2C_HandleTypeDef* hi2c;     // Handle for I2C
}ADS1xx5_I2C;




// Write the register
static void writeRegister(ADS1xx5_I2C *i2c, uint8_t reg, uint16_t value) {
	uint8_t pData[3] = { reg, (uint8_t) (value >> 8), (uint8_t) (value & 0xFF) };
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->m_i2cAddress, pData, 3, 10);
}

// Read the register
static uint16_t readRegister(ADS1xx5_I2C *i2c, uint8_t reg) {
	HAL_I2C_Master_Transmit(i2c->hi2c, i2c->m_i2cAddress, &reg, 1, 10);
	uint8_t pData[2] = { 0, 0 };
	HAL_I2C_Master_Receive(i2c->hi2c, i2c->m_i2cAddress, pData, 2, 10);
	return ((pData[0] << 8) | pData[1]);
}

// Check if we have correct connection.
static void ADSbegin(ADS1xx5_I2C *i2c) {
	if (HAL_I2C_IsDeviceReady(i2c->hi2c, i2c->m_i2cAddress, 10, 10) != HAL_OK)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // This MUST have GPIO PA5 ready to use - ERROR I2C - Wrong address
    
}

// Declare an ADS1015 structure
void ADS1015(ADS1xx5_I2C *i2c, I2C_HandleTypeDef *hi2c, uint8_t i2cAddress) {
	i2c->hi2c = hi2c;
	i2c->m_i2cAddress = i2cAddress << 1; //  It's Important to shift the address << 1
	i2c->m_conversionDelay = ADS1015_CONVERSIONDELAY;
    i2c->m_bitShift = 4;
	i2c->m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
	ADSbegin(i2c);
}

// Declare an ADS1115 structure
void ADS1115(ADS1xx5_I2C *i2c, I2C_HandleTypeDef *hi2c, uint8_t i2cAddress) {
	i2c->hi2c = hi2c;
	i2c->m_i2cAddress = i2cAddress << 1; //  It's Important to shift the address << 1
	i2c->m_conversionDelay = ADS1115_CONVERSIONDELAY;
	i2c->m_bitShift = 0;
	i2c->m_gain = GAIN_ONE; /* +/- 6.144V range (limited to VDD +0.3V max!) */
	ADSbegin(i2c);
}

/*
 * // The ADC input range (or gain) can be changed via the following
 // functions, but be careful never to exceed VDD +0.3V max, or to
 // exceed the upper and lower limits if you adjust the input range!
 // Setting these values incorrectly may destroy your ADC!
 //                                                                ADS1015  ADS1115
 //                                                                -------  -------
 // ADSsetGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
 // ADSsetGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
 // ADSsetGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
 // ADSsetGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
 // ADSsetGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
 // ADSsetGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
 */
void ADSsetGain(ADS1xx5_I2C *i2c, adsGain_t gain) {
	i2c->m_gain = gain;
}

// Get the gain
adsGain_t ADSgetGain(ADS1xx5_I2C *i2c) {
	return i2c->m_gain;
}

// Gets a single-ended ADC reading from the specified channel
uint16_t ADSreadADC_SingleEnded(ADS1xx5_I2C *i2c, uint8_t channel) {
	if (channel > 3) {
		return 0;
	}
    
	// Start with default values
	uint16_t config =
        ADS1015_REG_CONFIG_CQUE_NONE 			|   	// Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT 	|  	// Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW | 	// Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS 	| 	// 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_SINGLE;   	// Single-shot mode (default)
    
	// Set PGA/voltage range
	config |= i2c->m_gain;
    
	// Set single-ended input channel
	switch (channel) {
        case (0):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		break;
        case (1):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		break;
        case (2):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
		break;
        case (3):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
		break;
	}
    
	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
    
	// Write config register to the ADC
	writeRegister(i2c, ADS1015_REG_POINTER_CONFIG, config);
    
	// Wait for the conversion to complete
	HAL_Delay(i2c->m_conversionDelay);
    
	// Read the conversion results
	// Shift 12-bit results right 4 bits for the ADS1015
	return readRegister(i2c, ADS1015_REG_POINTER_CONVERT) >> i2c->m_bitShift;
}

/*
 * Reads the conversion results, measuring the voltage
 * difference between the P (AIN0) and N (AIN1) input.  Generates
 * a signed value since the difference can be either positive or negative.
 */
int16_t ADSreadADC_Differential_0_1(ADS1xx5_I2C *i2c) {
	// Start with default values
	uint16_t config =
        ADS1015_REG_CONFIG_CQUE_NONE 	 |   // Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT    |   // Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW   |   // Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD 	|   // Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS 	|   // 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_SINGLE;   	// Single-shot mode (default)
    
	// Set PGA/voltage range
	config |= i2c->m_gain;
    
	// Set channels
	config |= ADS1015_REG_CONFIG_MUX_DIFF_0_1; // AIN0 = P, AIN1 = N
    
	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
    
	// Write config register to the ADC
	writeRegister(i2c, ADS1015_REG_POINTER_CONFIG, config);
    
	// Wait for the conversion to complete
	HAL_Delay(i2c->m_conversionDelay);
    
	// Read the conversion results
	uint16_t res = readRegister(i2c, ADS1015_REG_POINTER_CONVERT) >> i2c->m_bitShift;
	if (i2c->m_bitShift == 0) {
		return (int16_t) res;
	} else {
		// Shift 12-bit results right 4 bits for the ADS1015,
		// making sure we keep the sign bit intact
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t) res;
	}
}

/*
 * Reads the conversion results, measuring the voltage
 * difference between the P (AIN2) and N (AIN3) input.  Generates
 * a signed value since the difference can be either positive or negative.
 */
int16_t ADSreadADC_Differential_2_3(ADS1xx5_I2C *i2c) {
	// Start with default values
	uint16_t config =
        ADS1015_REG_CONFIG_CQUE_NONE 	 |     // Disable the comparator (default val)
        ADS1015_REG_CONFIG_CLAT_NONLAT    | 	// Non-latching (default val)
        ADS1015_REG_CONFIG_CPOL_ACTVLOW   |     // Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD 	|     // Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS 	|     // 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_SINGLE;     	// Single-shot mode (default)
    
	// Set PGA/voltage range
	config |= i2c->m_gain;
    
	// Set channels
	config |= ADS1015_REG_CONFIG_MUX_DIFF_2_3; // AIN2 = P, AIN3 = N
    
	// Set 'start single-conversion' bit
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
    
	// Write config register to the ADC
	writeRegister(i2c, ADS1015_REG_POINTER_CONFIG, config);
    
	// Wait for the conversion to complete
	HAL_Delay(i2c->m_conversionDelay);
    
	// Read the conversion results
	uint16_t res = readRegister(i2c, ADS1015_REG_POINTER_CONVERT) >> i2c->m_bitShift;
	if (i2c->m_bitShift == 0) {
		return (int16_t) res;
	} else {
		// Shift 12-bit results right 4 bits for the ADS1015,
		// making sure we keep the sign bit intact 
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t) res;
	}
}

/*
 * Sets up the comparator to operate in basic mode, causing the
 * ALERT/RDY pin to assert (go from high to low) when the ADC
 * value exceeds the specified threshold.
 * This will also set the ADC in continuous conversion mode.
 */
void ADSstartComparator_SingleEnded(ADS1xx5_I2C *i2c, uint8_t channel, int16_t threshold) {
	// Start with default values
	uint16_t config =
        ADS1015_REG_CONFIG_CQUE_1CONV 	|   	// Comparator enabled and asserts on 1 match
        ADS1015_REG_CONFIG_CLAT_LATCH 	|   	// Latching mode
        ADS1015_REG_CONFIG_CPOL_ACTVLOW   | 	// Alert/Rdy active low   (default val)
        ADS1015_REG_CONFIG_CMODE_TRAD 	| 	// Traditional comparator (default val)
        ADS1015_REG_CONFIG_DR_1600SPS 	|	 // 1600 samples per second (default)
        ADS1015_REG_CONFIG_MODE_CONTIN    |  	// Continuous conversion mode
        ADS1015_REG_CONFIG_MODE_CONTIN;   	// Continuous conversion mode
    
	// Set PGA/voltage range
	config |= i2c->m_gain;
    
	// Set single-ended input channel
	switch (channel) {
        case (0):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
		break;
        case (1):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
		break;
        case (2):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
		break;
        case (3):
		config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
		break;
	}
    
	// Set the high threshold register
	// Shift 12-bit results left 4 bits for the ADS1015
	writeRegister(i2c, ADS1015_REG_POINTER_HITHRESH, threshold << i2c->m_bitShift);
    
	// Write config register to the ADC
	writeRegister(i2c, ADS1015_REG_POINTER_CONFIG, config);
}


// NOTE(rsb): In order to clear the comparator, we need to read the conversion results.
// NOTE(rsb): This function reads the last conversion results without changing the config value.


int16_t ADSgetLastConversionResults(ADS1xx5_I2C *i2c) {
	// Wait for the conversion to complete
	HAL_Delay(i2c->m_conversionDelay);
    
	// Read the conversion results
	uint16_t res = readRegister(i2c, ADS1015_REG_POINTER_CONVERT) >> i2c->m_bitShift;
	if (i2c->m_bitShift == 0) {
		return (int16_t) res;
	} else {
		// Shift 12-bit results right 4 bits for the ADS1015,
		// making sure we keep the sign bit intact
		if (res > 0x07FF) {
			// negative number - extend the sign to 16th bit
			res |= 0xF000;
		}
		return (int16_t) res;
	}
}


#endif //ADS1115_ADS1015_H
