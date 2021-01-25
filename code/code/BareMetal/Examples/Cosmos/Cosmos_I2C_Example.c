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
#include "RealtimeClock.h"
#include "SystemMonitor.h"
#include "TimerInterface.h"
#include "GlobalVariables.h"
#include "I2cExampleDefines.h"
#include "ReadSystemMonitor.h"
#include "Multiplexer.h"
#include "24AA128T.h"

#if SYSTEM == ALTERA_ARM_SOC
	#include "BootInterface.h"
#endif

//-------------------------------------------------------------------------------------------------
// File scope variables
//-------------------------------------------------------------------------------------------------

/// A little bit of left padding for when we're printing strings.
char LEFT_PADDING[] = "   ";


//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------

/**
 * \brief Perform required initialisation of the system resources used by the application.
 */

EN_RESULT InitaliseSystem()
{

	EN_RETURN_IF_FAILED(InitialiseTimer());

	EN_RETURN_IF_FAILED(InitialiseI2cInterface());

	EN_RETURN_IF_FAILED(SetupInterruptSystem());

	return EN_SUCCESS;
}

/**
 * \brief Perform required boot initialisation
 */

#if SYSTEM == ALTERA_ARM_SOC

	EN_RESULT InitaliseSystemBoot()
	{

		EN_RETURN_IF_FAILED(InitialiseBoot());

		return EN_SUCCESS;
	}

#else

	EN_RESULT InitaliseSystemBoot()
	{

		return EN_SUCCESS;
	}

#endif

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

/**
 * \brief Demonstrates how to set and read the real-time clock.
 */
EN_RESULT ReadRealtimeClock()
{
	 EN_PRINTF("\n\rReal Time Clock:\n\r");

    // Initialise the RTC.
    EN_RETURN_IF_FAILED(Rtc_Initialise());

    // Set the time and date, then wait a bit.
    EN_RETURN_IF_FAILED(Rtc_SetTime(11, 22, 32));
    EN_RETURN_IF_FAILED(Rtc_SetDate(22, 11, 10));
    SleepMilliseconds(1000);

    // Read back the time.
    int hour, minute, seconds;
    EN_RETURN_IF_FAILED(Rtc_ReadTime((int*)&hour, (int*)&minute, (int*)&seconds));

    // Read back the date.
    int year, month, day;
    EN_RETURN_IF_FAILED(Rtc_ReadDate((int*)&day, (int*)&month, (int*)&year));

    // Print time and date
    EN_PRINTF("%sTime: %d:%d:%d\n\r", LEFT_PADDING, hour, minute, seconds);
    EN_PRINTF("%sDate: %d.%d.%d\n\r", LEFT_PADDING, day, month, year);

	int temperature;
    if (!EN_FAILED(Rtc_ReadTemperature((int*)&temperature)) )
    {
    	EN_PRINTF("%sTemperature: %d Celsius\n\r", LEFT_PADDING, temperature);
    }

    return EN_SUCCESS;

}

EN_RESULT Test_Multiplexer()
{
	bool devicePresent;

	uint8_t* readBuffer;

	Mux_Initialise(&devicePresent);

	Mux_Read(readBuffer);
	EN_PRINTF("%sMultiplexer initial read: %x\n\r",LEFT_PADDING, *readBuffer);

	uint8_t writeBuffer = 0x01;
	Mux_Write(writeBuffer);
	EN_PRINTF("%sMultiplexer selected channel: %x\n\r", LEFT_PADDING, writeBuffer);

	SleepMilliseconds(100);

	Mux_Read(readBuffer);
	EN_PRINTF("%sMultiplexer read new configuration: %x\n\r",LEFT_PADDING, *readBuffer);

	return EN_SUCCESS;
}

EN_RESULT Test_UserEEPROM()
{
	EN_PRINTF("\n\rUser EEPROM test:\n\r");

	bool devicePresent;

	uint8_t* readBuffer;

	UserEEPROM_Read(readBuffer);

	UserEEPROM_Initialise(&devicePresent);

	EN_PRINTF("%sUser EEPROM initial read: %x\n\r",LEFT_PADDING, *readBuffer);

	uint8_t writeBuffer = 0xAD;
	UserEEPROM_Write(writeBuffer);
	EN_PRINTF("%sUser EEPROM written data: %x\n\r", LEFT_PADDING, writeBuffer);

	SleepMilliseconds(100);

	UserEEPROM_Read(readBuffer);
	EN_PRINTF("%sUser EEPROM read after write: %x\n\r",LEFT_PADDING, *readBuffer);

	return EN_SUCCESS;
}

int main()
{

	InitaliseSystemBoot();

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

    if (EN_FAILED(ReadRealtimeClock()))
    {
        EN_PRINTF("Error: Real-time clock read failed\n\r");
        return -1;
    }

    if (EN_FAILED(Test_Multiplexer()))
	{
		EN_PRINTF("Error: Multiplexer test failed\n\r");
		return -1;
	}

    if (EN_FAILED(ReadSystemMonitor()))
    {
        EN_PRINTF("Error: System monitor read failed\n\r");
        return -1;
    }

    if (EN_FAILED(Test_UserEEPROM()))
    {
        EN_PRINTF("Error: Test User EEPROM failed\n\r");
        return -1;
    }

    EN_PRINTF("\n\r== End of test ==\n\r");

	while (1);
    return 0;
}
