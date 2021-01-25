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

#include "ReadSystemMonitor.h"
#include "SystemMonitor.h"
#include "I2cInterface.h"
#include "UtilityFunctions.h" 
#include "SystemController.h" 

//-------------------------------------------------------------------------------------------------
// Function definitions
//-------------------------------------------------------------------------------------------------
EN_RESULT ReadSystemMonitor()
{
	/// A little bit of left padding for when we're printing strings.
	char LEFT_PADDING[] = "   ";

	// Read and display values from the current monitor
	EN_PRINTF("\n\rSystem Monitor:\n\r");

	// Perform any required initialisation.
	EN_RETURN_IF_FAILED(SystemMonitor_Initialise());

	// Get the voltage reading.

	//Measure the voltages for DA branch on the multiplexers
	SystemController_SetVmonSel(0);

	struct VoltageReadingSysMon voltReadingSel0[7] = {
		{"VMON_12V",    {47000000, 10000000}},
		{"VMON_3V3",    {10000000, 22000000}},
		{"VMON_OUT_A",  {10000000, 47000000}},
		{"VMON_OUT_B",  {10000000, 47000000}},
		{"VMON_CS_MOD", {0, 1}},
		{"VMON_CS_3V3", {0, 1}},
		{"VMON_5V",     {22000000, 10000000}}
	};

	int vBus;
	int cBus;
	int vRef;
	int i;
	int Rshunt;

	Rshunt = 10;

	//Read Current Sense VREF Voltage
	//EN_RETURN_IF_FAILED(SystemMonitor_ReadVoltage(6, &vRef, voltReading[6].VoltageDivResistors[0], voltReading[6].VoltageDivResistors[1]));
	vRef = 0;

	for (i = 0; i < 7; i++)
	{
		if(i ==4 || i== 5)
		{
			EN_RETURN_IF_FAILED(SystemMonitor_ReadCurrent(i, &cBus, Rshunt, vRef));
			EN_PRINTF("%s%-15s%s%d mA\n\r", LEFT_PADDING, voltReadingSel0[i].VoltageLabel, "Current = ", cBus);
		}
		else {
			EN_RETURN_IF_FAILED(SystemMonitor_ReadVoltage(i, &vBus, voltReadingSel0[i].VoltageDivResistors[0], voltReadingSel0[i].VoltageDivResistors[1]));
			EN_PRINTF("%s%-15s%s%d mV\n\r", LEFT_PADDING, voltReadingSel0[i].VoltageLabel, "Voltage = ", vBus);
		}
	}

	SystemController_SetVmonSel(1);
	struct VoltageReadingSysMon voltReadingSel1[7] = {
		{"VMON_A102",	{0, 1}},
		{"VMON_B8",  	{0, 1}},
		{"VMON_B167", 	{0, 1}},
		{"VMON_B168", 	{0, 1}},
		{"VMON_CS_A", 	{0, 1}},
		{"VMON_CS_B", 	{0, 1}},
		{"VREF_CS", 	{0, 1}}
	};

	for (i = 0; i < 7; i++)
	{
		if(i ==4 || i== 5)
		{
			EN_RETURN_IF_FAILED(SystemMonitor_ReadCurrent(i, &cBus, Rshunt, vRef));
			EN_PRINTF("%s%-15s%s%d mA\n\r", LEFT_PADDING, voltReadingSel1[i].VoltageLabel, "Current = ", cBus);
		}
		else {
			EN_RETURN_IF_FAILED(SystemMonitor_ReadVoltage(i, &vBus, voltReadingSel1[i].VoltageDivResistors[0], voltReadingSel1[i].VoltageDivResistors[1]));
			EN_PRINTF("%s%-15s%s%d mV\n\r", LEFT_PADDING, voltReadingSel1[i].VoltageLabel, "Voltage = ", vBus);
		}
	}

	SystemController_SetVmonSel(0);

	return EN_SUCCESS;
}
