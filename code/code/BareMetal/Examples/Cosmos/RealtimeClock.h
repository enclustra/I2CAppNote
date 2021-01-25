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
 * \brief Initialise the real-time clock.
 *
 * @return	Result code
 */
EN_RESULT Rtc_Initialise();


/**
 * \brief Read the time from the real-time clock.
 *
 * @param[out] pHour		Pointer to variable to receive hour value
 * @param[out] pMinutes		Pointer to variable to receive minutes value
 * @param[out] pSeconds		Pointer to variable to receive seconds value
 * @return					Result code
 */
EN_RESULT Rtc_ReadTime(int* pHour, int* pMinutes, int* pSeconds);


/**
 * \brief Set the RTC time.
 *
 * @param hour		Hour
 * @param minutes	Minutes
 * @param seconds	Seconds
 * @return			Result code
 */
EN_RESULT Rtc_SetTime(int hour, int minutes, int seconds);


/**
 * \brief Read the date from the RTC.
 *
 * @param[out] pDay		Pointer to variable to receive day value
 * @param[out] pMonth	Pointer to variable to receive month value
 * @param[out] pYear	Pointer to variable to receive year value
 * @return				Result code
 */
EN_RESULT Rtc_ReadDate(int* pDay, int* pMonth, int* pYear);


/**
 * \brief Set the date in the RTC.
 *
 * @param day		Day
 * @param month		Month
 * @param year		Year
 * @return			Result code
 */
EN_RESULT Rtc_SetDate(int day, int month, int year);


/**
 * \brief Read the temperature in degrees Celsius from the RTC.
 *
 * @param[out] pTemperature		Pointer to variable to receive temperature value
 * @return						Result code
 */
EN_RESULT Rtc_ReadTemperature(int* pTemperature);
