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

#include "I2cInterface.h"
#include "TimerInterface.h"
#include "UtilityFunctions.h" 
 
//-------------------------------------------------------------------------------------------------
// Directives, typedefs and constants
//-------------------------------------------------------------------------------------------------

#define SYSTEM_CONTROLLER_DEVICE_ADDRESS 0x0D
 
//-------------------------------------------------------------------------------------------------
// Register addresses
//------------------------------------------------------------------------------------------------- 
 
#define SYSTEM_CONTROLLER_REGISTER_ADDRESS_VMON_SEL    20
#define SYSTEM_CONTROLLER_REGISTER_ADDRESS_VMON_SEL_EN 21
 
 
//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------

EN_RESULT SystemController_SetVmonSel(int set_bit)
{

	uint8_t tempValue = 0;
	EN_RETURN_IF_FAILED(I2cRead(SYSTEM_CONTROLLER_DEVICE_ADDRESS,
			SYSTEM_CONTROLLER_REGISTER_ADDRESS_VMON_SEL,
			EI2cSubAddressMode_OneByte,
			1,
			(uint8_t*)&tempValue));

	//Set/Reset Bit 2 of Register 20 --> set Vmon_Sel to 1/0
	if (set_bit) {
		tempValue |= (1 << 2);
	}
	else {
		tempValue &= ~(1 << 2);
	}

	EN_RETURN_IF_FAILED(I2cWrite(SYSTEM_CONTROLLER_DEVICE_ADDRESS,
    		SYSTEM_CONTROLLER_REGISTER_ADDRESS_VMON_SEL,
    		EI2cSubAddressMode_OneByte,
    		&tempValue,
    		1));

    tempValue = 0;
	EN_RETURN_IF_FAILED(I2cRead(SYSTEM_CONTROLLER_DEVICE_ADDRESS,
			SYSTEM_CONTROLLER_REGISTER_ADDRESS_VMON_SEL_EN,
			EI2cSubAddressMode_OneByte,
			1,
			(uint8_t*)&tempValue));

	//Set Bit 2 of Register 21 --> set Vmon_Sel Enable to 1
	tempValue |= (1 << 2);

    EN_RETURN_IF_FAILED(I2cWrite(SYSTEM_CONTROLLER_DEVICE_ADDRESS,
    		SYSTEM_CONTROLLER_REGISTER_ADDRESS_VMON_SEL_EN,
    		EI2cSubAddressMode_OneByte,
    		&tempValue,
    		1));

	SleepMilliseconds(750);

}
