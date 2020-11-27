# Chapter 4 - U-boot
U-boot also offers a way of accessing, checking, reading and writing to devices via the I2C bus. With the command `i2c -help` all available commands and the corresponding syntax is shown. The [Cosmos XZQ10](https://www.enclustra.com/en/products/system-boards/cosmos-xzq10/) is used for showcasing the usage of some of these commands. The RTC present on the board is read with the help of the U-boot `I2C tools`. The command `i2c bus` lists all available I2C buses.

```
Zynq > i2c bus
Bus 0: zynq_0
```

With `i2c probe` all valid I2C addresses are listed. The device tree entry specifies the address of the RTC on the Cosmos XZQ10 as `0x6f` which corresponds to the serial console output.

```
Zynq > i2c probe
Valid chip addresses: 0D 20 23 2F 30 32 34 36 56 57 64 6F 70 74
```

As an example of how to read the first six registers of the RTC the following command is used: `i2c md 0x6f 0x00 0x06`. The address is given as 0x6f to read from the RTC, the start address for the read is `0x00` and `0x06` is the number of registers that are read (six in this case, corresponding to seconds, minutes, hours, day, month, year).

```
Zynq > i2c md 0x6f 0x00 0x06
0000: 34 57 97 05 02 20 4W...
```

**The next chapter of this application note is [Chapter 5 - Linux](./Chapter-5-Linux.md).**
