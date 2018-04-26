# StandardTransducer

This repository is dedicated to the v0.2 of Standard Transducer.

## Electronics

The architecture for v0.2 is articulated around one central board powered by an ATmega32u4 MCU relying on ChibiOS/Nil RTOS with LoRa communication and external boards for interfacing to extra sensors, OLED screens and Bluetooth.

The main board comes with the following features:

- 4 ports for digital sensors under 1-wire communication protocol.

- 7 ports for 2 analog sensors with current source or voltage source and differential or single out (this ports was based in pressure sensor MPX53D and temperature sensor PT100).

- 1 port for OLED I2C screen.
- 1 port for 3.7 V battery source or 5 - 12 Vdc @ 1 A.
- 1 port for antenna extension (433 and 868 MHz).
- 1 port for test power source (Vcc).
- 1 port for change power source 3.3 Vdc / 5-12 Vdc.
- 1 port for 868 MHz antenna with SMA connector.
- 1 port for 868 MHz antenna with U.fl connector.
- 1 port for serial Bluetooth.
- 4 buttons for local configuration.
- 1 pad for soldering LoRa Ra-1 (do not include if RN2483 is part of the design).
- 1 pad for soldering LoRa RN2483 (do not include if Ra-01 is part of the design).


There are already a few external extra modules:

~~- OLED I2C slave.~~

~~- pH & Conductommetry regulation I2C slave, also works as a stand alone (in development phase)~~
~~

~~- Pressure analog sensor.~~

~~- Temperature analog sensor.~~

~~- gas flux and mix control with analogic valves, also works as a stand alone (in development phase).~~

References:

-https://www.arduino.cc/en/Main/ArduinoBoardLeonardo.

-https://github.com/greiman/ChNil.
