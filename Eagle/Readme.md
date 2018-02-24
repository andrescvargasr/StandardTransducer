The main board comes with the following features:

- 3 pads with 3 ports for digital sensors under 1-wire communication protocol.

Port | Name | Description
---|----|---------------------------------------------------------------------------------
1 | Vsensors | Digital Voltage source for one-wire sensors.
2 | One-wire | Digital output of sensors.
3 | GND | Ground for digital sensors.

- 1 connector with 2x5 ports for 2 analog sensors with current source or voltage source, differential or single out (this ports was based in pressure sensor MPX53D and temperature sensor PT100) and one-wire sensors.

Port | Name | Sensor Number | Description
---|----|-------------------------------|---------------------------------------------------------------------------------
1 | D+3V3 |  | Voltage source for sensor 1.
2 | AGND |  | Analog ground for sensor 1 and voltage source.
3 | Vout+ | 1 | Positive differential voltage output of sensor 1.
4 | Vout- | 1 | Negative Differential voltage output of sensor 1.
5 | Iexc |  | Current source for sensor 2.
6 | AGND |  | Analog ground for sensor 2 and current source.
7 | Vanalog | 2 | Analog voltage output of sensor 2.
8 | GND |  | Ground for different digital sensors connect in sensor 3.
9 | Vsensors | | Digital Voltage source for one-wire sensors.
10 | One-wire | 3 | Digital output of sensors in port 10.

- 1 connector with 4 ports for OLED I2C screen.

Port | Name | Description
---|----|---------------------------------------------------------------------------------
1 | SDA | Serial Data.
2 | SCL | Serial Clock.
3 | LCDVCC | Voltage source.
4 | GND | Ground.

- 1 connector with 3 ports for 3.7 V battery source or 5 - 12 Vdc @ 1 A.

Port | Name | Description
---|----|---------------------------------------------------------------------------------
1 | Vdd | 5 - 12 V @ 1 A voltage source.
2 | +3V3 | Battery source.
3 | GND | Ground.

- 1 port for antenna extension (433 and 868 MHz).
- 1 port for test power source (Vcc).
- 1 jumper for change power source 3.3 Vdc to 5-12 Vdc.
- 1 port for 868 MHz antenna with SMA connector.
- 1 port for 868 MHz antenna with U.fl connector.
- 1 connector with 4 ports for serial Bluetooth.

Port | Name | Description
---|----|---------------------------------------------------------------------------------
1 | D+3V3 | Voltage source.
2 | TX2 | Transmission port for slave.
3 | RX2 | Reception port for slave.
4 | GND | Ground.

- 4 buttons for local configuration.
- 1 pad for soldering LoRa Ra-1 (do not include if RN2483 is part of the design).
- 1 pad for soldering LoRa RN2483 (do not include if Ra-01 is part of the design).

References:

- https://www.luisllamas.es/temperatura-liquidos-arduino-ds18b20/
- https://www.nxp.com/docs/en/data-sheet/MPX53.pdf
- https://www.acromag.com/sites/default/files/RTD_Temperature_Measurement_917A.pdf
- https://learn.adafruit.com/monochrome-oled-breakouts
- https://hetpro-store.com/TUTORIALES/bluetooth-hc-06-app-arduino/
- http://www.abatek.com/designguide/04Electrical.html
- https://www.arduino.cc/en/Main/ArduinoBoardLeonardo
