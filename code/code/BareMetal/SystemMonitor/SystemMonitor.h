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

#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------

#include "StandardIncludes.h"

//-------------------------------------------------------------------------------------------------
// Function declarations
//-------------------------------------------------------------------------------------------------

/**
 * \brief Initialise the system monitor,
 *
 * @return	Result code
 */
EN_RESULT SystemMonitor_Initialise();

/**
 * \brief Read a raw value from the system monitor
 *
 * @param[in] channel		Channel Index (0-6)
 * @param[out] pValue		Pointer to variable to receive raw value
 * @return					Result code
 */
EN_RESULT SystemMonitor_ReadValue(uint16_t channel, uint16_t* pValue);

/**
 * \brief Read a voltage in millivolt from the system monitor
 *
 * @param[in] channel		Channel Index (0-6)
 * @param[out] pVoltage		Pointer to variable to receive voltage
 * @param[in] RUpper		Voltage divider upper resistor (0 if no divider is used) in milliohms
 * @param[in] RLower		Voltage divider lower resistor (1 if no divider is used) in milliohms
 * @return					Result code
 */
EN_RESULT SystemMonitor_ReadVoltage(uint16_t channel, int* pVoltage, int RUpper, int RLower);

/**
 * \brief Read a current in millivolt from the system monitor
 *
 * @param[in] channel		Channel Index (0-6)
 * @param[out] pCurrent		Pointer to variable to receive voltage
 * @param[in] RShunt		Shunt resistor value in milliohms
 * @param[in] vRef			Reference Voltage of INA199A2 current sensor
 * @return					Result code
 */
EN_RESULT SystemMonitor_ReadCurrent(uint16_t channel, int* pCurrent, int RShunt, int vRef);
