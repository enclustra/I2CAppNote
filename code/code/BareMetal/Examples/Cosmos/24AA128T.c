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

#include "24AA128T.h"

//-------------------------------------------------------------------------------------------------
// Directives, typedefs and constants
//-------------------------------------------------------------------------------------------------

// 24AA128T I2C device address on Cosmos XZQ10
#define USER_EEPROM_DEVICE_ADDRESS 0x56

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------

// Try to read from register at address 0 to see if the device is present on the specified device address
EN_RESULT UserEEPROM_Initialise(bool* pDeviceIsPresent) {
	if (pDeviceIsPresent == NULL)
	    {
	        return EN_ERROR_NULL_POINTER;
	    }

	uint8_t readBuffer;
    if (EN_FAILED(
            I2cRead(USER_EEPROM_DEVICE_ADDRESS, 0, EI2cSubAddressMode_OneByte, sizeof(readBuffer), (uint8_t*)&readBuffer)))
    {
        *pDeviceIsPresent = false;
        EN_PRINTF("Device not present at address: 0x%x \n\r", USER_EEPROM_DEVICE_ADDRESS);
    }
    else
    {
        *pDeviceIsPresent = true;
        EN_PRINTF("Device present at address: 0x%x\n\r", USER_EEPROM_DEVICE_ADDRESS);
    }

	return EN_SUCCESS;
}

EN_RESULT UserEEPROM_Read(uint8_t* readBuffer) {
	EN_RETURN_IF_FAILED(I2cRead(USER_EEPROM_DEVICE_ADDRESS, 0x00, EI2cSubAddressMode_TwoBytes, 1, readBuffer));
	return EN_SUCCESS;
}

EN_RESULT UserEEPROM_Write(uint8_t writeBuffer) {

	EN_RETURN_IF_FAILED(I2cWrite(USER_EEPROM_DEVICE_ADDRESS, 0x00, EI2cSubAddressMode_TwoBytes, (uint8_t*)&writeBuffer, 1));

	return EN_SUCCESS;
}
