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
#include "I2cInterface.h"
#include "UtilityFunctions.h"

// Function declarations
//-------------------------------------------------------------------------------------------------

/**
 * \brief Initialize the clock generator.
 * @param	pDeviceIsPresent	check if device is present at the device address
 * @return	Result  code
 */
EN_RESULT ClkGen_Initialise(bool* pDeviceIsPresent);

/**
 * \brief Configure the clock generator
 *
 * Configures all registers of the clock generator with the data out of a header file.
 * This header file can be generated using the ClockBuilder Pro software available by SI
 *
 * @return					Result code
 */
EN_RESULT ClkGen_WriteData();

/**
 * \brief Read all data from the clock generator
 *
 * @return					Result code
 */
EN_RESULT ClkGen_ReadAllData();
