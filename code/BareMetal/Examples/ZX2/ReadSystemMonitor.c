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
				{"VCC_MAIN",    {47000000, 10000000}},
				{"VCC_3V3_MOD", {10000000, 22000000}},
				{"VCC_OUT",     {10000000, 47000000}},
				{"VCC_2V5",     {10000000, 47000000}},
				{"VCC_5V",      {22000000, 10000000}},
				{"VMON_CS_MOD", {0, 1}},
				{"VREF_CS",     {0, 1}}
			};
		
			int vBus;
			int cBus;
			int vRef;
			int i;
			int Rshunt;
		
			Rshunt = 10;
		
			//Read Current Sense VREF Voltage
			EN_RETURN_IF_FAILED(SystemMonitor_ReadVoltage(6, &vRef, voltReadingSel0[6].VoltageDivResistors[0], voltReadingSel0[6].VoltageDivResistors[1]));
		
			for (i = 0; i < 7; i++)
			{
				if(i== 5)
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

			EN_PRINTF("\nSystem controller change multiplexer output\n");

			struct VoltageReadingSysMon voltReadingSel1[4] = {
				{"VCC_P41",  {0, 1}},
				{"VCC_P42",  {0, 1}},
				{"VCC_P94",  {0, 1}},
				{"VCC_P198", {0, 1}}
			};
		
			for (i = 0; i < 4; i++)
			{
				EN_RETURN_IF_FAILED(SystemMonitor_ReadVoltage(i, &vBus, voltReadingSel1[i].VoltageDivResistors[0], voltReadingSel1[i].VoltageDivResistors[1]));
				EN_PRINTF("%s%-15s%s%d mV\n\r", LEFT_PADDING, voltReadingSel1[i].VoltageLabel, "Voltage = ", vBus);
		
			}
		
			SystemController_SetVmonSel(0);
		
			return EN_SUCCESS;
		}
