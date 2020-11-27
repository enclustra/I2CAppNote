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
#include "InterruptController.h"
#include "ModuleEeprom.h"
#include "SystemController.h"
#include "SystemMonitor.h"
#include "TimerInterface.h"
#include "I2cExampleDefines.h"
#include "ReadSystemMonitor.h"
#include "ClockGenerator.h"

//-------------------------------------------------------------------------------------------------
// File scope variables
//-------------------------------------------------------------------------------------------------

/// A little bit of left padding for when we're printing strings.
char LEFT_PADDING[] = "   ";

//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------

EN_RESULT InitaliseSystem()
{

	EN_RETURN_IF_FAILED(InitialiseTimer());

	EN_RETURN_IF_FAILED(InitialiseI2cInterface());

	EN_RETURN_IF_FAILED(SetupInterruptSystem());

	return EN_SUCCESS;
}

/**
 * \brief Read the module EEPROM and print the module configuration details.
 *
 * Enclustra modules all carry an EEPROM, which contains information such as module type, serial
 * number, MAC address, and information on the module configuration properties. This function
 * demonstrates how to read these properties.
 *
 * Some of the properties, such as the memory size or number of Ethernet ports, may be useful in
 * other parts of application code - in general, when a property is required in to be used
 * across multiple .c files, it should be declared once in GlobalVariables.h with the keyword
 * extern, and then defined once in a single .c file. In this example, g_
 */
EN_RESULT ReadEeprom()
{
    EN_PRINTF("EEPROM:\n\r");

    // Initialise the EEPROM.
    EN_RETURN_IF_FAILED(Eeprom_Initialise());

    // Read the EEPROM.
    EN_RETURN_IF_FAILED(Eeprom_Read());

    // After reading the EEPROM, the information is stored in its own translation unit - we can
    // query it using the EEPROM API functions.
    uint32_t serialNumber;
    ProductNumberInfo_t productNumberInfo;
    uint8_t macAddress[6];
    EN_RETURN_IF_FAILED(Eeprom_GetModuleInfo(&serialNumber, &productNumberInfo, (uint64_t*)&macAddress));

    EN_PRINTF("%s%-30s%s\n\r", LEFT_PADDING, "Module type", CONFIG_MODULE_NAME);
    EN_PRINTF("%s%-30s%d\n\r", LEFT_PADDING, "Revision", productNumberInfo.revisionNumber);
    EN_PRINTF("%s%-30s%d\n\r", LEFT_PADDING, "Serial number", serialNumber);
    EN_PRINTF("%s%-30s%02X:%02X:%02X:%02X:%02X:%02X\n\r",
              LEFT_PADDING,
              "MAC Address 0",
              macAddress[5],
              macAddress[4],
              macAddress[3],
              macAddress[2],
              macAddress[1],
              macAddress[0]);

    // Print the module configuration data.
    Eeprom_PrintModuleConfig();

/*	Info:
	//
    // Module specific-code to evaluate specific properties should be inserted below. Properties
    // can be retrieved by index - the indices themselves are defined for ease of use in
    // ModuleConfigConstants.h. The example below shows property retrieval for the Mercury ZX1
    // module.

    // Get the number of Gigabit Ethernet ports
    uint32_t gigabitEthernetPortCount = 0;
    Eeprom_GetModuleConfigPropertyValue(MERCURY_ZX1_CONFIG_PROPERTY_INDEX_GIGABIT_ETHERNET_PORT_COUNT,
                                        &gigabitEthernetPortCount);

    // Check if an RTC is present.
    uint32_t rtcEquipped = 0;
    Eeprom_GetModuleConfigPropertyValue(MERCURY_ZX1_CONFIG_PROPERTY_INDEX_REAL_TIME_CLOCK_EQUIPPED, &rtcEquipped);

    // Get the size of the DDR3L PS memory.
    uint32_t g_ddr3lPsSizeMb = 0;
    Eeprom_GetModuleConfigPropertyValue(MERCURY_ZX1_CONFIG_PROPERTY_INDEX_DDR3L_RAM_PS_SIZE_MB, &g_ddr3lPsSizeMb);
*/

    return EN_SUCCESS;
}

EN_RESULT ClockGeneratorTest() {
    bool devicePresentAtI2cAddress = false;

    //Clock generator test
    if (EN_FAILED(ClkGen_Initialise(&devicePresentAtI2cAddress)))
    {
        EN_PRINTF("Error: Clock generator not present \n\r");
        return -1;
    }

    // read initial configuration
    ClkGen_ReadAllData();

    // write configuration from the generated register file exported from ClockBuilder Pro
    ClkGen_WriteData();

    // read new configuration to confirm changes
    ClkGen_ReadAllData();

	return EN_SUCCESS;
}

int main() {

	EN_PRINTF("\n\r== Enclustra I2C test ==\n\r");

	if (EN_FAILED(InitaliseSystem()))
	{
		EN_PRINTF("Error: System initialization failed\n\r");
		return -1;
	}

	if (EN_FAILED(ReadEeprom()))
	{
		EN_PRINTF("Error: EEPROM read failed\n\r");
		return -1;
	}

    if (EN_FAILED(ReadSystemMonitor()))
    {
        EN_PRINTF("Error: System monitor read failed\n\r");
        return -1;
    }

    if (EN_FAILED(ClockGeneratorTest()))
    {
        EN_PRINTF("Error: Clock generator test failed\n\r");
        return -1;
    }

    EN_PRINTF("\n\r== End of test ==\r\n");
}
