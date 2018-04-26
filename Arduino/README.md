# Standard Tranducer Version 2.0

## Serial Options

| COMMAND | DESCRIPTION   |
| ------- | ------------- |
| h       | Help          |
| c       | Configuration | (s: Settings)
| u       | Utilities     |
| i       | I2C           |
| o       | One-wire      |
| p       | Parameters    |
| s       | Status        | (t: Status)

- h: (h)elp
Display principal commands for configuration & visualization of the Transducer.
- c: (c)onfiguration
Display all Parameter names and values.
- u: (u)tilities

| SUBCOMMAND | DESCRIPTION      |
| ---------- | ---------------- |
| uc         | Compact Settings |
| ue         | Epoch            |
| uf         | Free SRAM        |
| uq         | Qualifier        |
| ur         | Reset            |
| uz         | EEPROM           |

- i: (i)2c
Configuration for I2C communication, actually is only for controll of the OLED 128x32.
- o: (o)ne-wire
Configuration for One-wire sensors, actually is for Temperature & Humidity sensors DS18B20.
- p: (p)arameters
Display parameters of epoch, time and I2C.
- s: (s)tatus
Display flags: Status, Enabled and Error.

## Flags
- FLAG_STATUS
