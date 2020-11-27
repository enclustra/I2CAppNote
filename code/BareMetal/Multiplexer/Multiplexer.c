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

#include "Multiplexer.h"

//-------------------------------------------------------------------------------------------------
// Directives, typedefs and constants
//-------------------------------------------------------------------------------------------------

// PCA9647 I2C device address on Cosmos XZQ10
#define MULTIPLEXER_DEVICE_ADDRESS 0x74

// Configuration register read mask: only the 4 LSBs are relevant (bit 3 is enable bit, bit 2-0 are used for channel selection)
#define READ_CONFIGURATION_REGISTER_MASK 0x0F

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------

// Try to read from register at address 0 to see if the device is present on the specified device address
EN_RESULT Mux_Initialise(bool* pDeviceIsPresent) {
	if (pDeviceIsPresent == NULL)
	    {
	        return EN_ERROR_NULL_POINTER;
	    }

	uint8_t readBuffer;
    if (EN_FAILED(
            I2cRead(MULTIPLEXER_DEVICE_ADDRESS, 0, EI2cSubAddressMode_OneByte, sizeof(readBuffer), (uint8_t*)&readBuffer)))
    {
        *pDeviceIsPresent = false;
        EN_PRINTF("Device not present at address: 0x%x \n\r", MULTIPLEXER_DEVICE_ADDRESS);
    }
    else
    {
        *pDeviceIsPresent = true;
        EN_PRINTF("Device present at address: 0x%x \n\r", MULTIPLEXER_DEVICE_ADDRESS);
    }

	return EN_SUCCESS;
}

EN_RESULT Mux_Read(uint8_t* readBuffer) {

	// check the currently active channel by reading the contents of the configuration register
	EN_RETURN_IF_FAILED(I2cRead(MULTIPLEXER_DEVICE_ADDRESS, 0x00, EI2cSubAddressMode_OneByte, 1, (uint8_t*)&readBuffer));

	// only four LSBs hold relevant bits
	*readBuffer = *readBuffer & READ_CONFIGURATION_REGISTER_MASK;

	return EN_SUCCESS;
}

EN_RESULT Mux_Write(uint8_t writeBuffer) {

	// select the desired channel via writing to the configuration register

	// 0x08 to set bit 4th LSB, which is the output enable bit
	writeBuffer = writeBuffer | 0x08;

	EN_RETURN_IF_FAILED(I2cWrite(MULTIPLEXER_DEVICE_ADDRESS, 0x00, EI2cSubAddressMode_OneByte, (uint8_t*)&writeBuffer, 1));

	return EN_SUCCESS;
}
