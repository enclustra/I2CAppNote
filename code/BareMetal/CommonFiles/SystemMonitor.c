/**---------------------------------------------------------------------------------------------------
-- Copyright (c) 2020 by Enclustra GmbH, Switzerland.
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy of
-- this hardware, software, firmware, and associated documentation files (the
-- "Product"), to deal in the Product without restriction, including without
-- limitation the rights to use, copy, modify, merge, publish, distribute,
-- sublicense, and/or sell copies of the Product, and to permit persons to whom the
-- Product is furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Product.
--
-- THE PRODUCT IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
-- INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
-- PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
-- HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
-- OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
-- PRODUCT OR THE USE OR OTHER DEALINGS IN THE PRODUCT.
---------------------------------------------------------------------------------------------------
*/

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------

#include "SystemMonitor.h"
#include "I2cInterface.h"
#include "UtilityFunctions.h"
#include "TimerInterface.h"

//-------------------------------------------------------------------------------------------------
// Directives, typedefs and constants
//-------------------------------------------------------------------------------------------------

#define SYSTEM_MONITOR_DEVICE_ADDRESS 0x2F

//-------------------------------------------------------------------------------------------------
// Register addresses
//-------------------------------------------------------------------------------------------------

// Configuration Register
#define SYSTEM_MONITOR_REGISTER_ADDRESS_CONFIG 0x00

// Interrupt Status Register 1
#define SYSTEM_MONITOR_REGISTER_ADDRESS_INTR_STATUS_1 0x01

// Interrupt Status Register 2
#define SYSTEM_MONITOR_REGISTER_ADDRESS_INTR_STATUS_2 0x02

// Interrupt Mask Register 1
#define SYSTEM_MONITOR_REGISTER_ADDRESS_INTR_MASK_1 0x03

// Interrupt Mask Register 2
#define SYSTEM_MONITOR_REGISTER_ADDRESS_INTR_MASK_2 0x04

// Fan Divisor / RTS_OUT / OS Register
#define SYSTEM_MONITOR_REGISTER_ADDRESS_FAN_DIV 0x05

// OS Configuration/Temperature Resolution Register
#define SYSTEM_MONITOR_REGISTER_ADDRESS_OS_CONFIG 0x06

// Conversion Rate Register
#define SYSTEM_MONITOR_REGISTER_ADDRESS_CONV_RATE 0x07

// Channel Disable Register
#define SYSTEM_MONITOR_REGISTER_ADDRESS_CHAN_DISABLE 0x08

// Value RAM Base Address
#define SYSTEM_MONITOR_REGISTER_ADDRESS_VALUE_BASE 0x20

//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------

EN_RESULT SystemMonitor_Initialise()
{
    uint8_t Value = 0;

	/** Write interrupt mask register 1: Value = 0xFF disables all interrupt status bits */
	Value = 0xFF;
	EN_RETURN_IF_FAILED(I2cWrite(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_INTR_MASK_1,
								EI2cSubAddressMode_OneByte,
								&Value,
								1));

	/** Write interrupt mask register 2: Value = 0xFF disables interrupt status bits 0 to 6, setting bit 7 and bit 8 to 1 selects the one time interrupt mode for hot temperature and over temperature shutdown (OS) respectively */
	Value = 0xFF;
	EN_RETURN_IF_FAILED(I2cWrite(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_INTR_MASK_2,
								EI2cSubAddressMode_OneByte,
								&Value,
								1));

	/** Set OS Configuration/Temperature Resolution Register bit 4 to 1 in order to select the 11-bit plus sign resolution temperature conversion*/
	Value = 0x09;
	EN_RETURN_IF_FAILED(I2cWrite(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_OS_CONFIG,
								EI2cSubAddressMode_OneByte,
								&Value,
								1));

	// Set continuous conversion
	Value = 0x01;
	EN_RETURN_IF_FAILED(I2cWrite(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_CONV_RATE,
								EI2cSubAddressMode_OneByte,
								&Value,
								1));

	/** Start monitoring process by setting bit 0 of the Configuration Register to 1 */
	Value = 0x01;
	EN_RETURN_IF_FAILED(I2cWrite(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_CONFIG,
								EI2cSubAddressMode_OneByte,
								&Value,
								1));

	// Wait until the system monitor is ready
	SleepMilliseconds(500);

	return EN_SUCCESS;
}

EN_RESULT SystemMonitor_ReadValue(uint16_t channel, uint16_t* pValue)
{
	EN_RETURN_IF_FAILED(I2cRead(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_VALUE_BASE+channel,
								EI2cSubAddressMode_OneByte,
								2,
								(uint8_t*)pValue));

	return EN_SUCCESS;
}

EN_RESULT SystemMonitor_ReadVoltage(uint16_t channel, int* pVoltage, int RUpper, int RLower)
{
	uint16_t tempValue;
	EN_RETURN_IF_FAILED(I2cRead(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_VALUE_BASE+channel,
								EI2cSubAddressMode_OneByte,
								2,
								(uint8_t*)&tempValue));

	// Convert value to voltage
	tempValue = (tempValue << 8) | (tempValue >> 8);
    // Only the first 10 bits contain the value
	tempValue = tempValue >> 6;

	if ( RLower != 1 )
	{
        /** from voltage divider formula; the constant 2.5 corresponds to the 2.5 mV LSB weighting from the data sheet */
		*pVoltage = (tempValue * 2.5 * (RUpper + RLower) / RLower);
	} else {
		/**no divider to ground - the voltage is more or less the same (depending on the current through RUpper resistor)*/
		*pVoltage = (tempValue * 2.5 );
	}


	return EN_SUCCESS;
}

EN_RESULT SystemMonitor_ReadCurrent(uint16_t channel, int* pCurrent, int RShunt, int vRef)
{
	uint16_t tempValue = 0;
	EN_RETURN_IF_FAILED(I2cRead(SYSTEM_MONITOR_DEVICE_ADDRESS,
								SYSTEM_MONITOR_REGISTER_ADDRESS_VALUE_BASE+channel,
								EI2cSubAddressMode_OneByte,
								2,
								(uint8_t*)&tempValue));

	// Convert value to voltage
	tempValue = (tempValue << 8) | (tempValue >> 8);
    // Only the first 10 bits contain the value
	tempValue = tempValue >> 6;
	/** Calculate current value: I=U/R; 100 is the gain of the current shunt monitor TI INA 199; the value of RShunt is given in integer and needs to be converted to mOhm, thus the factor 0.001; vRef is typically 0 V*/
	*pCurrent = (((tempValue * 2.5) - vRef) / (100 * (0.001 * RShunt)));

	return EN_SUCCESS;
}

