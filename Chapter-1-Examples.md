# Examples
In this chapter examples for all I2C devices present on Enclustra hardware are presented. The sample code that is provided with this Application Note is ready to be used and can be copied to a custom project to handle I2C communication. Key parts of the code are explained in more detail in the respective example sections. These are excerpts from the code provided with this Application Note.

The following sections show example applications with Enclustra hardware for the different I2C devices. The used base boards and module combinations are chosen to provide an example for every device used on Enclustra hardware and described in more detail in the subsequent chapters. The code is available with the material accompanying this application note. There is no explicit example for the EEPROM Maxim DS28CN01U-A00+. The example code for the EEPROM ATSHA204A-MAHDA-T
already includes all the necessary adjustments for this case. More details about each device can be found in [Chapter 2 - I2C device description](./Chapter-2-I2CDeviceDescription.md) along with code excerpts of specific parts of the sample code provided.

**Warning: The code provided is written to be as vendor independent as possible. However, there are still unavoidable dependencies on vendor specific libraries. These libraries can change with different tool versions and thus the user needs to make sure that the libraries used in this example code are compatible with the tool version in use. Any necessary changes would need to be made in [`I2cInterface.h`](./code/I2cInterface.h).**

**Warning: Please note that some files in the respective example folders for each device are duplicates, e.g. `I2cInterface.h` is present in all example folders. This makes it easier to copy the contents of each example folder into the users project so that they can be used immediately.**

## 1.1 Mercury XU5 example
The [Mercury XU5](https://www.enclustra.com/en/products/system-on-chip-modules/mercury-xu5/) module combined with the [Mercury PE1](https://www.enclustra.com/en/products/base-boards/mercury-pe1-200-300-400/) base board is used as the platform for the majority of the I2C example implementations. The complete log file for the serial console output is also part of the provided material ([XU5_I2C_example-log.txt](./code/BareMetal/Examples/XU5/LogFiles/XU5_I2C_example-log.txt)). In the directory are also two more files related to the clock generator. The initial read as well as the read after the writing of the new configuration is exported from the serial console log file for easier comparison.

### 1.1.1 EEPROM ATSHA204A-MAHDA-T
The provided material for this application note includes code to read the stored information on the ATSHA204A-MAHDA-T. This includes module characteristics, available memory size and type as well as other useful data. The serial console excerpt is shown below.

```
EEPROM:
   Module type                   Mercury XU5
   Revision                      1
   Serial number                 131424
   MAC Address 0                 20:B0:F7:04:02:C0
   MPSoC type                    Xilinx Zynq UltraScale+ XCZU5EV
   MPSoC speed grade             2
   Temperature grade             Industrial
   Power grade                   Normal
   Gigabit Ethernet port count   2
   Real-time clock equipped      No
   Alternative MGT routing       No
   USB 2.0 port count            2
   DDR4 ECC RAM (PS) size (GB)   4
   DDR4 RAM (PL) size (MB)       1024
   eMMC flash size (GB)          16
   QSPI flash size (MB)          64
```

### 1.1.2 System monitor Texas Instruments LM96080CIMT/NOPB
The system monitor example shows the correct initialization of the resistor values for the different measurements points. The relevant code excerpts from [ReadSystemMonitor.c](./code/BareMetal/Examples/XU5/ReadSystemMonitor.c) for the definition of the resistor values for voltage and current measurements are shown below.

```c
struct VoltageReadingSysMon voltReadingSel0 [7] = {
    {"VMON_12V", {47000000 , 10000000}} ,
    {"VMON_3V3", {10000000 , 22000000}} ,
    {"VMON_OUT_A", {10000000 , 47000000}} ,
    {"VMON_OUT_B", {10000000 , 47000000}} ,
    {"VMON_CS_MOD", {0, 1}},
    {"VMON_CS_3V3", {0, 1}},
    {"VMON_5V", {22000000 , 10000000}}
};

            .
            .
            .

SystemController_SetVmonSel (1);
struct VoltageReadingSysMon voltReadingSel1 [7] = {
    {"VMON_A102", {0, 1}},
    {"VMON_B8", {0, 1}},
    {"VMON_B167", {0, 1}},
    {"VMON_B168", {0, 1}},
    {"VMON_CS_A", {0, 1}},
    {"VMON_CS_B", {0, 1}},
    {"VREF_CS", {0, 1}}
};
```

The serial console log output should look similar to this after running the example application.

```
System Monitor:
    VMON_12V Voltage = 11913 mV
    VMON_3V3 Voltage = 3330 mV
    VMON_OUT_A Voltage = 2561 mV
    VMON_OUT_B Voltage = 1810 mV
    VMON_CS_MOD Current = 1022 mA
    VMON_CS_3V3 Current = 25 mA
    VMON_5V Voltage = 5016 mV
    VMON_A102 Voltage = 852 mV
    VMON_B8 Voltage = 1202 mV
    VMON_B167 Voltage = 905 mV
    VMON_B168 Voltage = 850 mV
    VMON_CS_A Current = 0 mA
    VMON_CS_B Current = 52 mA
    VREF_CS Voltage = 0 mV
```

In this case, the different reference voltages and currents present on the board are measured. The system controller is used to switch between different voltage and current monitoring measurement points.

### 1.1.3 Clock generator Silicon Labs Si5338
The example for the clock generator first reads the initial configuration that is present on the board after checking if the device is indeed present at the specified I2C address. In the next step the configuration generated by the [ClockBuilder Pro software](https://www.silabs.com/developers/clockbuilder-pro-software) and exported in a C code style register map file is written to the board. After the new configuration has been written, a second read is performed of all the configuration registers to check whether the values have changed from the initial read in order to confirm the new configuration. The provided material includes a [LogFiles](./code/BareMetal/Examples/XU5/LogFiles) folder which shows the serial console output. A file containing the initial read values is given as well as a file containing the read values after writing the new configuration to the device.

## 1.2 Mars ZX2 example
The [Mars ZX2](https://www.enclustra.com/en/products/system-on-chip-modules/mars-zx2/) module in combination with the [Mars EB1](https://www.enclustra.com/en/products/base-boards/mars-eb1/) base board is used to show examples for the I2C devices that are not covered by the previous example. This includes the RTC and the system controller.

### 1.2.1 RTC PCF85063A
The sample code initializes the RTC chip and sets an arbitrary time and date. Afterwards, the values for date and time are read and displayed on the serial console.

```
Real Time Clock:
    Time: 11:22:33
    Date: 22.11.10
```

### 1.2.2 System controller Lattice LCMXO2-4000HC-6MG132I
The usage of the system controller is demonstrated in combination with the system monitor by switching to measure a different set of voltages. The serial console output is given below.

```
System Monitor:
    VCC_MAIN Voltage = 12098 mV
    VCC_3V3_MOD Voltage = 3294 mV
    VCC_OUT Voltage = 1803 mV
    VCC_2V5 Voltage = 2501 mV
    VCC_5V Voltage = 5008 mV
    VMON_CS_MOD Current = 470 mA
    VREF_CS Voltage = 70 mV

    System controller change multiplexer output

    VCC_P41 Voltage = 1365 mV
    VCC_P42 Voltage = 1002 mV
    VCC_P94 Voltage = 1807 mV
    VCC_P198 Voltage = 1222 mV
```

The point where the system controller is used to change between different measurement points is marked by a comment.

## 1.3 Cosmos XZQ10 system board example
The [Cosmos XZQ10](https://www.enclustra.com/en/products/system-boards/cosmos-xzq10/) system board is used to show examples for the I2C Multiplexer and the other RTC chip used on Enclustra hardware.

### 1.3.1 RTC ISL12020M
The sample code for the ISL12020M initializes the device and sets an arbitrary date and time. Afterwards, a read is performed to read back and confirm the setting of time and date. In addition the temperature is read and converted with helper functions to a decimal value and printed out to the serial console.

```
Real Time Clock:
    Time: 11:22:33
    Date: 22.11.10
    Temperature: 39 Celsius
```

### 1.3.2 8-channel bus multiplexer NXP PCA9547
The I2C multiplexer can be used to select different channels and expand the number of devices that can be accessed on the I2C bus. To select a different channel the configuration register has to be written with the desired channel. Only the four LSBs are relevant (LSB is the output enable bit).

### 1.3.3 EEPROM 24AA128T-I/MNY
The sample code for the user EEPROM writes data to the EEPROM and then reads it back to validate the process. Please refer to the provided source code for more details. The serial console output is shown here:

```
User EEPROM test:
    Device present at address: 0x56
    User EEPROM initial read: DE
    User EEPROM written data: AD
    User EEPROM read after write: AD
```

## References
* [EEPROM: Microchip ATSHA204A](http://ww1.microchip.com/downloads/en/DeviceDoc/ATSHA204A-Data-Sheet-40002025A.pdf)
* [EEPROM: Maxim DS28CN01](https://datasheets.maximintegrated.com/en/ds/DS28CN01.pdf)
* [EEPROM: Microchip 24AA128T-I/MNY](http://ww1.microchip.com/downloads/en/DeviceDoc/24AA128-24LC128-24FC128-128K-I2C-Serial-EEPROM-DS.pdf)
* [RTC: Renesas ISL12020M](https://www.renesas.com/us/en/www/doc/datasheet/isl12020m.pdf)
* [RTX: NXP PCF85063A](https://www.nxp.com/docs/en/data-sheet/PCF85063A.pdf)
* [Sytem Monitor: Texas Instruments LM96080](https://www.ti.com/lit/ds/symlink/lm96080.pdf)
* [Clock Generator: Silicon Labs Si5338](https://www.silabs.com/documents/public/data-sheets/Si5338.pdf)
* [Multiplexer: NXP PCA9547](https://www.nxp.com/docs/en/data-sheet/PCA9547.pdf)

**The next chapter of this application note is [Chapter 2 - I2C device description](./Chapter-2-I2CDeviceDescription.md).**