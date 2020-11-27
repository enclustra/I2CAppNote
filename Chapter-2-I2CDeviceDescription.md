# Chapter 2 - I2C device description
This chapter gives an overview over all I2C devices present on Enclustra hardware. Links to the data sheets of each device are provided in the [References](Chapter-2-I2CDeviceDescription.md#references) section.

## 2.1 EEPROM
An EEPROM is non-volatile memory that can be used to store relatively small amounts of data. It is possible to erase and reprogram individual bytes. One example application is the storage of module information as is done on the majority of Enclustra devices.

### 2.1.1 - ATSHA204A-MAHDA-T
This is the EEPROM chip commonly used on Enclustra hardware. It can be used for key storage, miscellaneous read/write data, read-only, secret data, consumption logging and security configuration. The access to the various memory sections can be restricted in a number of ways. Enclustra uses this EEPROM to store module information such as serial number for example. The maximum communication speed of this device using the I2C bus is 1 Mbps.

### 2.1.2 - Maxim DS28CN01U-A00+
Some Enclustra hardware products are equipped with DS28CN01U-A00+ EEPROM as an alternative to the ATSHA204A-MAHDA-T. The maximum communication speed of this device using the I2C bus is 400 kbps.

### 2.1.3 - Microchip 24AA128T-I/MNY
The Mircochip 24AA128T-I/MNY is an EEPROM completely available for user data. Enclustra does not store any information on this EEPROM by default.

## 2.2 RTC
An RTC is a low power solution to keep accurate time in a system. There are two different RTC chips used on Enclustra products described below.

### 2.2.1 - ISL12020M
The ISL12020M provides separate registers for hours, minutes and seconds as well as calendar registers with automatic leap year detection. A temperature sensor is also available.

### 2.2.2 - PCF85063A
The PCF85063A also provides time and calendar functions, but no temperature sensor. Leap year detection is also integrated.

## 2.3 System Controller Lattice LCMXO2-4000HC-6MG132I
The Lattice LCMXO2-4000HC-6MG132I is a PLD that is used for board configuration in Enclustra hardware. The system controller can be used to control the system monitor analog multiplexers. This way it is possible to access different measurement readings by changing the multiplexer control values via I2C through the system controller. Reprogramming the system controller voids the warranty of the Enclustra board. For more information about the capabilities of the system controller, please contact [support@enclustra.com](support@enclustra.com) for more information.

## 2.4 System Monitor Texas Instruments LM96080CIMT/NOPB
The TI LM96080CIMT/NOPB is capable of measuring up to seven positive voltages and local temperature amongst other hardware monitoring characteristics. The configuration of the system monitor is done via I2C. Standard Mode (100 kbps) and Fast Mode (400 kbps) are supported.

## 2.5 Clock generator Silicon Labs Si5338
The SI5338 can be used to generate up to four independent clock signals. It can be used to replace different frequency crystal oscillators or operate as a frequency translator. The configuration of the clocks is completely independent to support various signal formats and supply voltages. For reliable communication with the Si5338 the hold time specified in the data sheet has to be observed and changes to the hardware in use made accordingly. Altera/Intel devices specifically are in need of adjusting the hold time setting. The adjustment can be made by writing the value specified in the data sheet for the hold time to the `ic_sda_tx_hold` bit field of the `ic_sda_hold` register of the I2C controller. For more details about the hold time requirements for the clock generator device, please refer to the [data sheet](https://www.silabs.com/documents/public/data-sheets/Si5338.pdf).

## 2.6 8-channel bus multiplexer NXP PCA9547
The PCA9547 is an 8-channel I2C bus multiplexer. A programmable control register determines the active SDx/SCx channel. Only one channel can be active at a time. On power on, channel 0 is connected allowing immediate master and slave communication. This chip also provides voltage level translation between several voltages.

## References
* [EEPROM: Microchip ATSHA204A](http://ww1.microchip.com/downloads/en/DeviceDoc/ATSHA204A-Data-Sheet-40002025A.pdf)
* [EEPROM: Maxim DS28CN01](https://datasheets.maximintegrated.com/en/ds/DS28CN01.pdf)
* [EEPROM: Microchip 24AA128T-I/MNY](http://ww1.microchip.com/downloads/en/DeviceDoc/24AA128-24LC128-24FC128-128K-I2C-Serial-EEPROM-DS.pdf)
* [RTC: Renesas ISL12020M](https://www.renesas.com/us/en/www/doc/datasheet/isl12020m.pdf)
* [RTX: NXP PCF85063A](https://www.nxp.com/docs/en/data-sheet/PCF85063A.pdf)
* [Sytem Monitor: Texas Instruments LM96080](https://www.ti.com/lit/ds/symlink/lm96080.pdf)
* [Clock Generator: Silicon Labs Si5338](https://www.silabs.com/documents/public/data-sheets/Si5338.pdf)
* [Multiplexer: NXP PCA9547](https://www.nxp.com/docs/en/data-sheet/PCA9547.pdf)

**The next chapter of this application note is [Chapter 3 - Bare metal](./Chapter-3-BareMetal.md).**
