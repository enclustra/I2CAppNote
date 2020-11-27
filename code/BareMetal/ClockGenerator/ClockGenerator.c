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

#include "ClockGenerator.h"
#include "Si5338_register_map.h"

//-------------------------------------------------------------------------------------------------
// Directives, typedefs and constants
//-------------------------------------------------------------------------------------------------

// Si5338 I2C default device address
#define CLOCK_GENERATOR_DEVICE_ADDRESS 0x70

// Masks for clock generator configuration
#define LOS_MASK 0x04
#define LOCK_MASK 0x15

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------

// Try to read from register at address 0 to see if the device is present on the specified device address
EN_RESULT ClkGen_Initialise(bool* pDeviceIsPresent) {
	if (pDeviceIsPresent == NULL)
	    {
	        return EN_ERROR_NULL_POINTER;
	    }

	uint8_t readBuffer;
    if (EN_FAILED(
            I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 0, EI2cSubAddressMode_OneByte, sizeof(readBuffer), (uint8_t*)&readBuffer)))
    {
        *pDeviceIsPresent = false;
        EN_PRINTF("Device not present at address: 0x%x \n\r", CLOCK_GENERATOR_DEVICE_ADDRESS);
    }
    else
    {
        *pDeviceIsPresent = true;
        EN_PRINTF("Device present at address: 0x%x \n\r", CLOCK_GENERATOR_DEVICE_ADDRESS);
    }

	return EN_SUCCESS;
}

EN_RESULT ClkGen_ReadAllData() {
	uint8_t readBuffer [511];
	uint8_t readBuffer1 [255];
	uint8_t readBuffer2 [255];
	uint8_t writeBuffer;

	// There are 352 registers to access in the Si5338
	int NumberOfBytes = 352;

	// Read first page from 0 to end
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 0x00, EI2cSubAddressMode_OneByte, 256, (uint8_t*)&readBuffer1));

	// Set PAGE_SEL to second page
	SleepMilliseconds(200);
	writeBuffer = 0x01;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 0xFF, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));
	SleepMilliseconds(200);

	// Read second page from 0 to end
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 0x00, EI2cSubAddressMode_OneByte, 256, (uint8_t*)&readBuffer2));

	// Set to first stage
	writeBuffer = 0x00;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 0xFF, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	// Concatenate the two read pages and print the data
	memcpy(readBuffer, readBuffer1, sizeof(uint8_t)*256);
	memcpy(readBuffer + 256, readBuffer2, sizeof(uint8_t)*256);

	for(int i=0; i<=NumberOfBytes; i++) {
		EN_PRINTF("Address: %d; Content: %x \n\r", i, readBuffer[i]);
	}

	return EN_SUCCESS;
}

EN_RESULT ClkGen_WriteData() {
	uint8_t writeBuffer;
	uint8_t readBuffer;

	uint8_t currentValue;
	uint8_t clearCurrentValue;
	uint8_t clearNewValue;
	uint8_t newValue;

	uint8_t temp;

	uint8_t fcal1;
	uint8_t fcal2;

	Reg_Data currentEntry;

	/** Start at the top of the I2C programming procedure figure of the Si5338 data sheet */

	// Disable outputs: OEB_ALL=1; reg230[4]
	writeBuffer = 0x10;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 230, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	// Pause LOL: DIS_LOL=1; reg241[7]
	writeBuffer = 0xE5;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 241, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	/** Write all register values from the generated register map file to the Si5338 */

	EN_PRINTF("Get each value and mask and apply it to the Si5338 \n\r");

	// Get each value and mask and apply it to the Si5338
	for(int counter = 0; counter < NUM_REGS_MAX; counter++) {
		currentEntry = Reg_Store[counter];

		// If a mask is 0x00 all the bits in the register are reserved and can not be changed

		if(currentEntry.Reg_Mask != 0x00) {
			if(currentEntry.Reg_Mask == 0xFF) {
				// do a regular I2C write to the register at the specified address with the value from the generated register map file
				writeBuffer = currentEntry.Reg_Val;
				EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, currentEntry.Reg_Addr, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));
				EN_PRINTF("Address: %d; Value: %x \n\r", currentEntry.Reg_Addr, currentEntry.Reg_Val);
			}
			else {
				// do a read-modify-write using I2C and bit-wise operations

				// get the current value from the device at thh register located at the specified address
				EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, currentEntry.Reg_Addr, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
				currentValue = readBuffer;

				// clear the bits that are allowed to be accessed in the current value of the register
				clearCurrentValue = currentValue & ~currentEntry.Reg_Mask;

				// clear the bits in the desired data that are not allowed to be accessed
				clearNewValue = currentEntry.Reg_Val & currentEntry.Reg_Mask;

				// combine the cleared values to get the new value to write to the desired register
				newValue = clearNewValue | clearCurrentValue;
				writeBuffer = newValue;
				EN_PRINTF("Address: %d; Value: %x \n\r", currentEntry.Reg_Addr, newValue);
				EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, currentEntry.Reg_Addr, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));
			}
			SleepMilliseconds(200);
		}
	}

	/** Validate input clock status: input clock are validated with the LOS alarms.
	 * See register 218 to determine which LOS should be monitored.
	 * In the example configuration here the xtal input in IN1 and IN2 are checked.
	 * Change the LOS_MASK if using other inputs.
	 */

	// Check register 218 responsible for tracking LOL until input clock is valid
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 218, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	temp = readBuffer & LOS_MASK;
	while(temp != 0) {
		EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 218, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
		temp = readBuffer & LOS_MASK;
	}

	EN_PRINTF("Input clock is valid \n\r");

	// Configure PLL for locking: FCAL_OVRD_EN=0; reg49[7]
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 49, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	writeBuffer = readBuffer & 0x7F;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 49, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	// Initiate locking of PLL: SOFT_RESET = 1; reg246[1]
	writeBuffer = 0x02;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 246, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	EN_PRINTF("PLL locking initiated \n\r");

	// Wait at least 25 ms
	SleepMilliseconds(50);

	// Restart LOL: DIS_LOL = 0; reg241[7]; set reg241 = 0x65
	writeBuffer = 0x65;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 241, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	// Check if PLL is locked: PLL is locked when PLL_LOL, SYS_CAL and all other alarms are cleared
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 218, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	temp = readBuffer & LOCK_MASK;
	while(temp != 0) {
		EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 218, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
		temp = readBuffer & LOCK_MASK;
	}

	EN_PRINTF("PLL is locked \n\r");

	/** Copy FCAL values to active registers as follows:
	 * 237[1:0] to 47[1:0]
	 * 236[7:0] to 46[7:0]
	 * 235[7:0] to 45[7:0]
	 * Set 47[7:2] = 000101b
	 */
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 235, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 45, EI2cSubAddressMode_OneByte, (uint8_t*)&readBuffer, 1));

	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 236, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 46, EI2cSubAddressMode_OneByte, (uint8_t*)&readBuffer, 1));

	// clear bits 0 and 1 from 47 and combine with bit 0 and 1 from 237
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 47, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	fcal1 = readBuffer;
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 237, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	fcal2 = readBuffer;
	writeBuffer = (fcal1 & 0xFC) | (fcal2 & 0x03);
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 47, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	// Set PLL to use FCAL values: FCAL_OVRD_EN = 1; reg49[7]
	EN_RETURN_IF_FAILED(I2cRead(CLOCK_GENERATOR_DEVICE_ADDRESS, 49, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));
	writeBuffer = readBuffer | 0x80;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 49, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	// If using down spread check the I2C programming procedure in the I2C application note or the Si5338 data sheet at this stage to make the necessary adjustment

	// Enable outputs: OEB_ALL = 0; reg230[4]
	writeBuffer = 0x00;
	EN_RETURN_IF_FAILED(I2cWrite(CLOCK_GENERATOR_DEVICE_ADDRESS, 230, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	EN_PRINTF("Outputs are enabled \n\r");

	return EN_SUCCESS;
}
