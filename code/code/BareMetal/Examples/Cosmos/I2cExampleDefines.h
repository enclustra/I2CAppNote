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

#if TARGET_MODULE == MARS_ZX2
	#define CONFIG_MODULE_NAME                                     MARS_ZX2_MODULE_NAME
#elif TARGET_MODULE == MARS_ZX3
	#define CONFIG_MODULE_NAME                                     MARS_ZX3_MODULE_NAME
#elif TARGET_MODULE == MERCURY_ZX1
	#define CONFIG_MODULE_NAME                                     MERCURY_ZX1_MODULE_NAME
#elif TARGET_MODULE == MERCURY_ZX5
	#define CONFIG_MODULE_NAME                                     MERCURY_ZX5_MODULE_NAME
#elif TARGET_MODULE == MERCURY_XU1
	#define CONFIG_MODULE_NAME                                     MERCURY_XU1_MODULE_NAME
#elif TARGET_MODULE == MERCURY_KX1
	#define CONFIG_MODULE_NAME                                     MERCURY_KX1_MODULE_NAME
#elif TARGET_MODULE == MERCURY_KX2
	#define CONFIG_MODULE_NAME                                     MERCURY_KX2_MODULE_NAME
#elif TARGET_MODULE == MARS_AX3
	#define CONFIG_MODULE_NAME                                     MARS_AX3_MODULE_NAME
#elif TARGET_MODULE == MERCURY_SA1
	#define CONFIG_MODULE_NAME                                     MERCURY_SA1_MODULE_NAME
#elif TARGET_MODULE == MERCURY_SA2
	#define CONFIG_MODULE_NAME                                     MERCURY_SA2_MODULE_NAME
#elif TARGET_MODULE == MERCURY_AA1
	#define CONFIG_MODULE_NAME                                     MERCURY_AA1_MODULE_NAME
#elif TARGET_MODULE == MARS_XU3
	#define CONFIG_MODULE_NAME                                     MARS_XU3_MODULE_NAME
#elif TARGET_MODULE == COSMOS_XZQ10
	#define CONFIG_MODULE_NAME                                     COSMOS_XZQ10_MODULE_NAME
#elif TARGET_MODULE == MARS_MA3
	#define CONFIG_MODULE_NAME                                     MARS_MA3_MODULE_NAME
#elif TARGET_MODULE == MERCURY_XU5
	#define CONFIG_MODULE_NAME                                     MERCURY_XU5_MODULE_NAME
#elif TARGET_MODULE == MERCURY_XU7
	#define CONFIG_MODULE_NAME                                     MERCURY_XU7_MODULE_NAME
#elif TARGET_MODULE == MERCURY_XU8
	#define CONFIG_MODULE_NAME                                     MERCURY_XU8_MODULE_NAME
#elif TARGET_MODULE == MERCURY_XU9
	#define CONFIG_MODULE_NAME                                     MERCURY_XU9_MODULE_NAME
#endif
