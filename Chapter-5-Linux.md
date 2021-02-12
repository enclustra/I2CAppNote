# Chapter 5 - Linux
Enclustra provides a build environment for building Linux systems for Xilinx and Intel Enclustra hardware ([Enclustra Build Environment (EBE)](https://www.enclustra.com/en/products/tools/linux-build-environment/)). Enclustra also provides Petalinux BSPs for Enclustra hardware. Regardless of the Linux build tools chosen to create a Linux system the necessary adjustments to the device tree and the necessary drivers for the I2C devices used on Enclustra hardware are described in this section. Enclustra also provides a detailed guide for loading and integrating kernel modules into a custom Linux system ((ask [support@enclustra.com] for details)).

## 5.1 - EEPROM
There is no Linux driver available for the DS28CN01U-A00+ Secure EEPROM. However, the default Linux I2C commands can still be used to read from the Secure EEPROMs. Please refer to section [5.7 Linux I2C tools](Chapter-5-Linux.md#57-linux-i2c-tools) for further details. There are two different drivers available for the ATSHA204A-MAHDA-T, one is part of the mainline Linux kernel ([Link](https://github.com/torvalds/linux/blob/master/drivers/crypto/atmel-sha204a.c)). The other option is [this one](https://github.com/cryptotronix/atsha204-i2c). The second one can also be used to read from the OTP region.

### 5.1.1 24AA128T-I/MNY
For the 24AA128T-I/MNY a generic kernel driver can be used.

### 5.1.2 Kernel module integration
For a detailed description for Linux driver integration please refer to the Enclustra EBE guide (ask [support@enclustra.com] for details). The driver is found in the kernel configuration under: `Device Drivers -> Misc Devices -> EEPROM support -> I2C EEPROMs/RAMs/ROMs` from most vendors.

### 5.1.3 Device tree bindings
The device tree binding is shown in the following excerpt [UserEEPROM_AT24.dtsi](./code/Linux/dt-bindings/UserEEPROM_AT24.dtsi)

```c
&i2c0 {
    at24: at24@56 {
    compatible = "atmel ,24 c32";
    reg = <0x56 >;
    pagesize = <32>;
    };
};
```
The device tree node is under the `i2c0` entry.

### 5.1.4 Check successful integration
Kernel boot messages can be observed to check if the integration of the driver was successful.

```
[ 0.775221] at24 0 -0056: 4096 byte 24c32 EEPROM , writable , 32 bytes/ write
```

## 5.2 - RTC
In order to access the RTC in a Linux system the necessary drivers and device tree entries have to be made during system build.

### 5.2.1 Kernel module integration
For a detailed description for Linux driver integration please refer to the Enclustra EBE guide (ask [support@enclustra.com] for details). For kernel configuration the driver for the respective device can be found under: `Device Drivers -> Real Time Clock`. Enable `Intersil ISL12022` or `NXP PCF85063` depending on the hardware in use (The `ISL12022` is can be used for the `ISL12020M`). Make the necessary changes to device tree.

### 5.2.2 Device tree bindings
The following excerpt ([RTC.dtsi](./code/Linux/dt-bindings/RTC.dtsi)) shows the device tree bindings for the two RTC chips used on Enclustra hardware.

```c
&i2c0 {
    status = "okay";
    /* Both RTCs disabled by default , choose one */
    pcf85063: pcf85063@51 {
        status = "disabled";
        compatible = "nxp ,pcf85063";
        reg = <0x51 >;
    };
    isl12022: isl12022@68 {
        status = "disabled";
        compatible = "isil ,isl12022";
        reg = <0x6f >;
    };
};
```

By default both entries are disabled as it depends on the specific hardware in use and needs to be changed manually by setting the status of the respective device from ”disabled” to ”okay”. The compatible string specifies the driver that needs to be loaded corresponding to the driver chosen in the previous step. The reg value is the I2C device address. A list of all I2C addresses can be found in the respective user manual of the hardware in use.

### Check successful integration
After rebuilding the kernel with the necessary adjustments the module should be loaded during Linux boot. If everything is done correctly, there should be some messages during Linux boot indicating the loading of the specified device driver.

```
1 [ 2.274825] −rtcisl12022 −0006f: rtc core: registered −rtcisl12022 as rtc0
2 [ 2.449563] −rtcisl12022 −0006f: setting system clock to −−20000101 00:00:00 UTC (946684800)
```

## 5.3 - System Controller Lattice LCMXO2-4000HC-6MG132I
In terms of access over I2C only the possibility to change which values are monitored by the system monitor is present. There is no proper driver as of yet for the system controller configuration used in Enclustra hardware. However, Linux offers the possibility of user space access to I2C devices as well as command line commands (`i2cget`, `i2cset`, etc. See [5.7 Linux I2C tools](Chapter-5-Linux.md#57-linux-i2c-tools) for more details). In this way the system controller can still be accessed.

## 5.4 - System Monitor LM96080CIMT/NOPB
To integrate the system monitor into a Linux system the following adjustments have to be made.

### 5.4.1 Kernel module integration
The necessary driver for the system monitor can be found in: `Device Drivers -> Hardware Monitoring support`. Enable `National Semiconductor LM80 and LM96080`. Make the necessary changes to the device tree next.

### 5.4.2 Device tree bindings
The following excerpt ([SystemMonitor.dtsi](./code/Linux/dt-bindings/SystemMonitor.dtsi)) shows the device tree binding for the system monitor used on Enclustra hardware.

```c
&i2c0 {
    lm96080@2f {
    status = "okay";
    compatible = "lm80";
    reg = <0x2f >;
    };
};
```

The compatible string specifies the driver that needs to be loaded corresponding to the driver chosen in the previous step. The reg value is the I2C device address. A list of all I2C addresses can be found in the respective user manual of the hardware in use. As the device is accessed via I2C the device tree definition is under the `i2c0` device tree node.

## 5.5 Clock generator Silicon Labs Si5338
The integration of the clock generator follows the same steps for Linux integration.

### 5.5.1 Kernel module integration
The necessary driver for the clock generator can be found in: `Device Drivers -> Common Clock Framework`. Enable `Clock driver for SiLabs 5338`.

### 5.5.2 Device tree bindings
The device tree binding for the Si5338 depends on the configuration of the clock generator. For all the possible parameters and settings check the provided documentation ([Si5338.dtsi](./code/Linux/dt-bindings/Si5338.dtsi)). An example device tree excerpt is shown here:

```c
#include <dt -bindings/clock/clk -si5338.h>

&i2c0 {
    si5338@70 {
        compatible = silabs ,si5338;
        reg = 0x70;
        #address -cells = 1;
        #size -cells = 0;
        #clock -cells = 1;

        clocks = 0, 0, &ref24 , 0, 0;
        clock -names = xtal , in12 , in3 , in4 , in56; 4A list of all I2C addresses can be found in the respective  user     anual of the hardware in use D-0000-203-006 35 / 43 Version 01, 05.11.2020

        silab ,ref -source = SI5338_REF_SRC_CLKIN3;
        silab ,fb -source = SI5338_FB_SRC_NOCLK;
        silab ,pll -source = SI5338_PFD_IN_REF_REFCLK;

        output
        clkout0 {
            reg = 0;
            silabs ,drive -config = 3V3_LVDS;
            silabs ,clock -source = SI5338_OUT_MUX_MSN;
            silabs ,disable -state = SI5338_OUT_DIS_HIZ;
            clock -frequency = 125000000;
        };
        clkout1 {
            reg = 1;
            silabs ,drive -config = 3V3_LVDS;
            silabs ,clock -source = SI5338_OUT_MUX_MSN;
            silabs ,disable -state = SI5338_OUT_DIS_HIZ;
            clock -frequency = 125000000;
        };
        clkout2 {
            reg = 2;
            silabs ,drive -config = 3V3_LVDS;
            silabs ,clock -source = SI5338_OUT_MUX_MSN;
            silabs ,disable -state = SI5338_OUT_DIS_HIZ;
            clock -frequency = 125000000;
        };
        clkout3 {
            reg = 3;
            silabs ,drive -config = 3V3_LVDS;
            silabs ,clock -source = SI5338_OUT_MUX_MSN;
            silabs ,disable -state = SI5338_OUT_DIS_HIZ;
            clock -frequency = 125000000;
        };
    };
};
```

Necessary definitions are the correct clock sources as well as output clocks according to the clock generator configuration.

## 5.6 8-channel bus multiplexer NXP PCA9547
For a successful Linux integration the correct driver needs to be loaded and the device tree entries have to be set. All devices that are connected via the multiplexer need to be defined in the device tree.

### 5.6.1 Kernel module integration
The necessary driver for the NXP PCA9547 is under the following node in the kernel configuration: `Device drivers -> I2C support -> Multiplexer I2C chip support`. Enable `Philips PCA954x I2C Mux/Switches`.

### 5.6.2 Device tree bindings
For the device tree binding the I2C multiplexer has to be defined under the `i2c` node and all devices which are connected through the I2C multiplexer are a sub node of the multiplexer itself. An example ([Mux.dtsi](./code/Linux/dt-bindings/Mux.dtsi)) is shown in here:

```c
i2c0 {
    i2c_mux: i2c_mux@74 {
        compatible = "nxp ,pca9547";
        reg = <0x74 >;
        address -cells = <1>;
        size -cells = <0>;

        2c_mux@0 {
            address -cells = <1>;
            #size -cells = <0>;
            reg = <0>;

            pcal6416@20 {
                compatible = "ti ,tca6416";
                reg = <0x20 >;
            };

            lm96080@2f {
                compatible = "lm80";
                reg = <0x2f >;
            };

            si5338@70 {
                compatible = "silabs ,si5338";
                reg = <0x70 >;
                #address -cells = <1>;
                #size -cells = <0>;
                #clock -cells = <1>;

                clocks = <0>, <0>, <&ref24 >, <0>, <0>;
                clock -names = "xtal", "in12", "in3", "in4", "in56";

                silab ,ref -source = <SI5338_REF_SRC_CLKIN3 >;
                silab ,fb -source = <SI5338_FB_SRC_NOCLK >;
                silab ,pll -source = <SI5338_PFD_IN_REF_REFCLK >;

                /* output */
                clkout0 {
                    reg = <0>;
                    silabs ,drive -config = "3V3_LVDS";
                    silabs ,clock -source = <SI5338_OUT_MUX_MSN >;
                    silabs ,disable -state = <SI5338_OUT_DIS_HIZ >;
                    clock -frequency = <125000000 >;
                };
                clkout1 {
                    reg = <1>;
                    silabs ,drive -config = "3V3_LVDS";
                    silabs ,clock -source = <SI5338_OUT_MUX_MSN >;
                    silabs ,disable -state = <SI5338_OUT_DIS_HIZ >;
                    clock -frequency = <125000000 >;
                };
                clkout2 {
                    reg = <2>;
                    silabs ,drive -config = "3V3_LVDS";
                    silabs ,clock -source = <SI5338_OUT_MUX_MSN >;
                    silabs ,disable -state = <SI5338_OUT_DIS_HIZ >;
                    000-203-006 37 / 43 Version 01, 05.11.2020
                    clock -frequency = <125000000 >;
                };
                clkout3 {
                    reg = <3>;
                    silabs ,drive -config = "3V3_LVDS";
                    silabs ,clock -source = <SI5338_OUT_MUX_MSN >;
                    silabs ,disable -state = <SI5338_OUT_DIS_HIZ >;
                    clock -frequency = <125000000 >;
                };
            };
        };
    };
};
```

## 5.7 Linux I2C tools
Linux offers user space access to I2C devices through different tools. These provide a variety of functions for debugging purposes such as listing all I2C devices present on a bus, get and set functions for individual registers of the I2C devices and a dump function which reads all registers of a device on an I2C bus (For a detailed documentation and syntax please refer to the [I2C-tools documentation](https://manpages.debian.org/testing/i2c-tools/index.html)). As an example the I2C tools are demonstrated on the [Cosmos XZQ10](https://www.enclustra.com/en/products/system-boards/cosmos-xzq10/) system board. The device inspected is the RTC ISL12020M. The command `i2cdetect -l` lists all I2C buses on the system.

```
root@Cosmos :~# i2cdetect -l
i2c-3   i2c     i2c-0-mux (chan_id 2)       I2C adapter
i2c-1   i2c     i2c-0-mux (chan_id 0)       I2C adapter
i2c-8   i2c     i2c-0-mux (chan_id 7)       I2C adapter
i2c-6   i2c     i2c-0-mux (chan_id 5)       I2C adapter
i2c-4   i2c     i2c-0-mux (chan_id 3)       I2C adapter
i2c-2   i2c     i2c-0-mux (chan_id 1)       I2C adapter
i2c-0   i2c     Cadence I2C at e0004000     I2C adapter
i2c-7   i2c     i2c-0-mux (chan_id 6)       I2C adapter
i2c-5   i2c     i2c-0-mux (chan_id 4)       I2C adapter
```

In case of the Cosmos XZQ10 you can see the buses of the 8-channel multiplexer and the Cadence I2C bus. The RTC is present on the Cadence bus with the label `i2c-0`. 

The command `i2cdetect -y -r 0` can then be used to check all addresses on bus number `0`.

```
root@Cosmos :~# i2cdetect -y -r 0
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00: -- -- -- -- -- -- -- -- -- -- 0d -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: 20 -- -- 23 -- -- -- -- -- -- -- -- -- -- -- UU
30: 30 -- 32 -- 34 -- 36 -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- 56 57 -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- UU
70: 70 -- -- -- UU -- -- --
```

A value of `UU` means that probing of this address was skipped as it is currently in use by a driver. This strongly suggests there is a chip at this address. For the RTC the address is `0x6f` as defined in the schematic and the device tree. That is why at this address there is a `UU`.

With the command `i2cdump -f 0 0x6f` the complete contents of the RTC registers can be read. According to the [data sheet](https://www.renesas.com/us/en/www/doc/datasheet/isl12020m.pdf) the first three registers are seconds, minutes, hours and the next three are day, month and year.

```
root@Cosmos :~# i2cdump -f 0 0x6f
No size specified (using byte -data access)
WARNING! This program can confuse your I2C bus , cause data loss and worse!
I will probe file /dev/i2c -0, address 0x6f , mode byte
Continue? [Y/n] y
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f 0123456789abcdef
00: 51 02 95 05 02 20 03 06 41 00 00 15 47 1f 15 00 Q???? ??A..?G??.
10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
20: 00 00 00 00 00 00 00 00 00 00 00 00 08 64 00 00 ............?d..
30: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
80: 51 02 95 05 02 20 03 06 41 00 00 15 47 1f 15 00 Q???? ??A..?G??.
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
a0: 00 00 00 00 00 00 00 00 00 00 00 00 08 64 00 00 ............?d..
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
```

Individual registers can also be accessed via `i2cget -f 0 0x6f 0x01`. In this case the register with the address `0x01` is read, which corresponds to the minutes value.

```
root@Cosmos :~# i2cget -f 0 0x6f 0x01
WARNING! This program can confuse your I2C bus , cause data loss and worse! I will read from device file /dev/i2c -0, chip address 0x6f , data address 0x01 , using read byte data.
Continue? [Y/n] y
0x05
```

The time can also be checked with `cat /sys/class/rtc/rtc0/time` which gives the following output in this case: `15:05:21`. In this way the register value and the actual time value can be verified.

To verify that the register indeed has been set, the get command can be used. The serial log output looks as follows:

```
root@Cosmos :~# i2cset -f 0 0x6f 0x01 0x20 
WARNING! This program can confuse your I2C bus , cause data loss and worse! I will write to device file /dev/i2c-0, chip address 0x6f , data address 0x01 , data 0x20 , mode byte.
Continue? [Y/n] y
root@Cosmos :~# i2cget -f 0 0x6f 0x01
WARNING! This program can confuse your I2C bus , cause data loss and worse! I will read from device file /dev/i2c-0, chip address 0x6f , data address 0x01 , using read byte data.
Continue? [Y/n] y
0x20
root@Cosmos :~# cat /sys/class/rtc/rtc0/time
15:20:20
```
